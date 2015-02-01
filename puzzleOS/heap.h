/*
 *  heap.h
 *  puzzleOS
 *
 *  Created by Dmitry on 03.05.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#ifndef _HEAP_H_
#define _HEAP_H_

#include "ordered_array.h"

#define KHEAP_START			0xC0000000
#define KHEAP_INITIAL_SIZE	0x00500000
#define HEAP_INDEX_SIZE		0x20000
#define HEAP_MAGIC			0xbadc0de
#define HEAP_MIN_SIZE		0x70000

typedef struct {
	uint32 magic;   // Magic number, used for error checking and identification.
	uint8 is_hole;   // 1 if this is a hole. 0 if this is a block.
	size_t size;    // size of the block, including the end footer.
} header_t;

typedef struct {
	uint32 magic;     // Magic number, same as in header_t.
	header_t *header; // Pointer to the block header.
} footer_t;

typedef struct {
	ordered_array_t index;
	uint32 start_address; // The start of our allocated space.
	uint32 end_address;   // The end of our allocated space. May be expanded up to max_address.
	uint32 max_address;   // The maximum address the heap can be expanded to.
	uint8 supervisor;     // Should extra pages requested by us be mapped as supervisor-only?
	uint8 readonly;       // Should extra pages requested by us be mapped as read-only?
} heap_t;

heap_t *create_heap(uint32 start, uint32 end, uint32 max, uint8 supervisor, uint8 readonly);

void *alloc(uint32 size, uint8 page_align, heap_t *heap);
void free(void *p, heap_t *heap);


void *kmalloc_ex(uint32 size, int align, uint32 *phys_addr);
void *kmalloc(uint32 size);

void kfree(void *p);

extern heap_t *kheap;

#endif
