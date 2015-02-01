/*
 *  heap.c
 *  puzzleOS
 *
 *  Created by Dmitry on 03.05.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#include "puzzle.h"
#include "vmm.h"

extern uint32 placement_address;

heap_t *kheap = NULL;

static int32 find_smallest_hole(uint32 size, uint32 page_align, heap_t *heap) {
	uint32 i = 0;
	while (i < heap->index.size) {
		header_t *header = (header_t *)lookup_ordered_array(i, &heap->index);
	
		if (page_align) {
			uint32 location = (uint32)header;
			int32 offset = 0;
			if ((location + sizeof(header_t)) & 0x00000FFF)
				offset = 0x1000 - (location + sizeof(header_t)) % 0x1000;
			int32 hole_size = (int32)header->size - offset;

			if (hole_size >= (int32)size)
				break;
		} else if (header->size >= size)
			break;
		i++;
	}
	
	if (i == heap->index.size)
		return -1;
	else
		return i;
}

static uint8 header_t_predicate(void *a, void *b) {
	return (((header_t *)a)->size < ((header_t *)b)->size) ? 1 : 0;
}

heap_t *create_heap(uint32 start, uint32 end_addr, uint32 max, 
					uint8 supervisor, uint8 readonly) {
	
	heap_t *heap = (heap_t *)kmalloc(sizeof(heap_t));
	
	heap->index = place_ordered_array((uint32 *)start, HEAP_INDEX_SIZE, &header_t_predicate);
	start += sizeof(ordered_array_item_t) * HEAP_INDEX_SIZE;
	
	if (start & 0x00000FFF) {
		start &= 0xFFFFF000;
		start += 0x1000;
	}
	
	heap->start_address = start;
	heap->end_address = end_addr;
	heap->max_address = max;
	heap->supervisor = supervisor;
	heap->readonly = readonly;
	
	header_t *hole = (header_t *)start;
	hole->size = end_addr - start;
	hole->magic = HEAP_MAGIC;
	hole->is_hole = 1;
	insert_ordered_array(hole, &heap->index);
	
	return heap;
}

static void expand(uint32 new_size, heap_t *heap) {
	
	if (new_size & 0x00000FFF) {
		new_size &= 0xFFFFF000;
		new_size += 0x1000;
	}
	
	uint32 old_size = heap->end_address-heap->start_address;
	uint32 i = old_size;
	while (i < new_size) {
		vmm_alloc_page(vmm_create_page(heap->start_address + i,
									   (heap->readonly) ? 1 : 0,
									   (heap->supervisor) ? 0 : 1,
									   current_page_dir));
		i += VMM_PAGE_SIZE;
	}
	heap->end_address = heap->start_address + new_size;
}

static uint32 contract(uint32 new_size, heap_t *heap) {
	if (new_size & 0x1000) {
		new_size &= 0x1000;
		new_size += 0x1000;
	}
	
	if (new_size < HEAP_MIN_SIZE)
		new_size = HEAP_MIN_SIZE;
	uint32 old_size = heap->end_address-heap->start_address;
	uint32 i = old_size - 0x1000;
	while (new_size < i) {
		vmm_free_page(vmm_get_page(heap->start_address + i, current_page_dir));
		i -= 0x1000;
	}
	heap->end_address = heap->start_address + new_size;
	return new_size;
}

void *alloc(uint32 size, uint8 page_align, heap_t *heap) {
	
    uint32 new_size = size + sizeof(header_t) + sizeof(footer_t);
    
    int32 iterator = find_smallest_hole(new_size, page_align, heap);
	
    if (iterator == -1) {
        uint32 old_length = heap->end_address - heap->start_address;
        uint32 old_end_address = heap->end_address;
		
        expand(old_length+new_size, heap);
        uint32 new_length = heap->end_address-heap->start_address;
		
        iterator = 0;
        uint32 idx = -1; uint32 value = 0x0;
        while (iterator < heap->index.size) {
            uint32 tmp = (uint32)lookup_ordered_array(iterator, &heap->index);
            if (tmp > value) {
                value = tmp;
                idx = iterator;
            }
            iterator++;
        }
		

        if (idx == -1) {
            header_t *header = (header_t *)old_end_address;
            header->magic = HEAP_MAGIC;
            header->size = new_length - old_length;
            header->is_hole = 1;
            footer_t *footer = (footer_t *) (old_end_address + header->size - sizeof(footer_t));
            footer->magic = HEAP_MAGIC;
            footer->header = header;
            insert_ordered_array((void*)header, &heap->index);
        } else {
            header_t *header = lookup_ordered_array(idx, &heap->index);
            header->size += new_length - old_length;
            footer_t *footer = (footer_t *)((uint32)header + header->size - sizeof(footer_t));
            footer->header = header;
            footer->magic = HEAP_MAGIC;
        }
        return alloc(size, page_align, heap);
    }
	
    header_t *orig_hole_header = (header_t *)lookup_ordered_array(iterator, &heap->index);
    uint32 orig_hole_pos = (uint32)orig_hole_header;
    uint32 orig_hole_size = orig_hole_header->size;
    
    if (orig_hole_size - new_size < sizeof(header_t) + sizeof(footer_t)) {
        size += orig_hole_size - new_size;
        new_size = orig_hole_size;
    }
	
    if (page_align && (orig_hole_pos & 0xFFFFF000)) {
        uint32 new_location   = orig_hole_pos + VMM_PAGE_SIZE - (orig_hole_pos&0xFFF) - sizeof(header_t);
        header_t *hole_header = (header_t *)orig_hole_pos;
        hole_header->size     = VMM_PAGE_SIZE - (orig_hole_pos&0xFFF) - sizeof(header_t);
        hole_header->magic    = HEAP_MAGIC;
        hole_header->is_hole  = 1;
        footer_t *hole_footer = (footer_t *) ( (uint32)new_location - sizeof(footer_t) );
        hole_footer->magic    = HEAP_MAGIC;
        hole_footer->header   = hole_header;
        orig_hole_pos         = new_location;
        orig_hole_size        = orig_hole_size - hole_header->size;
    } else {
        remove_ordered_array(iterator, &heap->index);
    }
	
    header_t *block_header  = (header_t *)orig_hole_pos;
    block_header->magic     = HEAP_MAGIC;
    block_header->is_hole   = 0;
    block_header->size      = new_size;
    
    footer_t *block_footer  = (footer_t *) (orig_hole_pos + sizeof(header_t) + size);
    block_footer->magic     = HEAP_MAGIC;
    block_footer->header    = block_header;
	
   
    if (orig_hole_size - new_size > 0) {
        header_t *hole_header = (header_t *) (orig_hole_pos + sizeof(header_t) + size + sizeof(footer_t));
        hole_header->magic    = HEAP_MAGIC;
        hole_header->is_hole  = 1;
        hole_header->size     = orig_hole_size - new_size;
        footer_t *hole_footer = (footer_t *) ( (uint32)hole_header + orig_hole_size - new_size - sizeof(footer_t) );
        if ((uint32)hole_footer < heap->end_address) {
            hole_footer->magic = HEAP_MAGIC;
            hole_footer->header = hole_header;
        }
        insert_ordered_array((void*)hole_header, &heap->index);
    }
    
    return (void *) ( (uint32)block_header+sizeof(header_t) );
}

void free(void *p, heap_t *heap) {
    if (p == 0)
        return;
	
    header_t *header = (header_t*) ( (uint32)p - sizeof(header_t) );
    footer_t *footer = (footer_t*) ( (uint32)header + header->size - sizeof(footer_t) );
	
	header->is_hole = 1;
	
    char do_add = 1;
	
    footer_t *test_footer = (footer_t *)((uint32)header - sizeof(footer_t));
    if (test_footer->magic == HEAP_MAGIC && test_footer->header->is_hole == 1) {
        uint32 cache_size = header->size; 
        header = test_footer->header;     
        footer->header = header;          
        header->size += cache_size;       
        do_add = 0;                       
    }
	
    header_t *test_header = (header_t*) ((uint32)footer + sizeof(footer_t));
    if (test_header->magic == HEAP_MAGIC && test_header->is_hole) {
        header->size += test_header->size;
        test_footer = (footer_t *)((uint32)test_header + test_header->size - sizeof(footer_t));
        footer = test_footer;
        
        uint32 iterator = 0;
        while ((iterator < heap->index.size) &&
			   (lookup_ordered_array(iterator, &heap->index) != (void*)test_header))
            iterator++;
		
        remove_ordered_array(iterator, &heap->index);
    }
	

    if ((uint32)footer+sizeof(footer_t) == heap->end_address) {
        uint32 old_length = heap->end_address-heap->start_address;
        uint32 new_length = contract((uint32)header - heap->start_address, heap);
	
        if (header->size - (old_length-new_length) > 0) {
            
            header->size -= old_length-new_length;
            footer = (footer_t*) ( (uint32)header + header->size - sizeof(footer_t) );
            footer->magic = HEAP_MAGIC;
            footer->header = header;
        } else {
            uint32 iterator = 0;
            while ( (iterator < heap->index.size) &&
				   (lookup_ordered_array(iterator, &heap->index) != (void*)test_header) )
                iterator++;
            if (iterator < heap->index.size)
                remove_ordered_array(iterator, &heap->index);
        }
    }
	
    if (do_add == 1)
        insert_ordered_array((void*)header, &heap->index);
}

void *kmalloc_ex(uint32 size, int align, uint32 *phys_addr) {
	if (kheap != NULL) {
        void *addr = alloc(size, (uint8)align, kheap);
        if (phys_addr != 0) {
            page_t *page = vmm_get_page((uint32)addr, current_page_dir);
            *phys_addr = (page->p_addr * VMM_PAGE_SIZE) + ((uint32)addr & 0xFFF);
        }
        return addr;
    } else {
		if (align && (placement_address & 0x00000FFF)) {
			placement_address &= 0xFFFFF000;
			placement_address += 0x1000;
		}
		if (phys_addr) {
			*phys_addr = placement_address;
		}
		void *tmp = (void *)placement_address;
		placement_address += size;
		
		memset(tmp, 0, size);
		
		return tmp;
	}
}

void *kmalloc(uint32 size) {
    return kmalloc_ex(size, 0, 0);
}

void kfree(void *p) {
    free(p, kheap);
}
