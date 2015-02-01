/*
 *  vmm.c
 *  puzzleOS
 *
 *  Created by Dmitry on 13.05.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#include "puzzle.h"
#include "vmm.h"
#include "pmm.h"
#include "interrupts.h"
#include "exeptions.h"

#define VMM_ALIGN_CHECK(addr) if (addr % VMM_PAGE_SIZE) \
								PANIC("Addres is not page-aligned!");

extern uint32 kernel_end;

uint32 placement_address = (uint32)&kernel_end;

page_directory_t *kernel_page_dir = NULL;
page_directory_t *current_page_dir = NULL;

static void page_fault_exeption_handler(exeption_info_t *exeption_info_t);

void vmm_init() {
	kprintf("Initializing physical memory manager...\n");
	pmm_init(20*1024*1024);
	kprintf("Initializing virtual memory manager...\n");
	kernel_page_dir = vmm_create_page_directory();
	
	kprintf("Kernel page directory at 0x%x\n", kernel_page_dir);
	
	int i = 0;
		
	for (i = KHEAP_START; i < KHEAP_START+KHEAP_INITIAL_SIZE; i += VMM_PAGE_SIZE)
		vmm_create_page(i, 1, 1, kernel_page_dir);
	
	i = 0;
	while (i < placement_address + VMM_PAGE_SIZE) {
		vmm_alloc_page(vmm_create_page(i, 1, 1, kernel_page_dir));
		i += VMM_PAGE_SIZE;
	}

	for (i = KHEAP_START; i < KHEAP_START+KHEAP_INITIAL_SIZE; i += VMM_PAGE_SIZE)
		vmm_alloc_page(vmm_get_page(i, kernel_page_dir));
	
	
	vmm_set_current_page_directory(kernel_page_dir);
	enable_paging();
	
	set_interrupt_handler(14, page_fault_exeption_handler);
}

page_directory_t *vmm_create_page_directory() {
	uint32 phys;
	page_directory_t *dir = kmalloc_ex(sizeof(page_directory_t), 1, &phys);
	memset(dir, 0, sizeof(page_directory_t));
	uint32 offset = (uint32)dir->tables_phys - (uint32)dir;
	dir->phys_addr = phys + offset;
	return dir;
}

page_directory_t *vmm_copy_page_directory(page_directory_t *dir, page_directory_t *src) {
	
	int i;
	for (i = 0; i < VMM_TABLES_PER_DIRECTORY; i++)
	{
		if (!src->tables[i])
			continue;
		
		if (kernel_page_dir->tables[i] == src->tables[i]) {
			dir->tables[i] = src->tables[i];
			dir->tables_phys[i] = src->tables_phys[i];
        } else {
			// Copy the table.
			//uint32 phys;
			//dst->tables[i] = vmm_clone_table(src->tables[i], &phys);
			//dst->tables_phys[i] = phys | 0x07;
			
			kprintf("Oh no! Tables copy!!");
			for(;;);
        }
	}
	return dir;
}

//page_table_t *clone_table(page_table_t *src, uint32 *physAddr)
//{
//	// Make a new page table, which is page aligned.
//	page_table_t *table = (page_table_t*)kmalloc_ex(sizeof(page_table_t), 1,physAddr);
//	// Ensure that the new table is blank.
//	memset(table, 0, sizeof(page_directory_t));
//	
//	// For every entry in the table...
//	int i;
//	for (i = 0; i < 1024; i++)
//	{
//		if (!src->pages[i].p_addr)
//			continue;
//		// Get a new frame.
//		alloc_frame(&table->pages[i], 0, 0);
//		// Clone the flags from source to destination.
//		if (src->pages[i].present) table->pages[i].present = 1;
//		if (src->pages[i].rw)      table->pages[i].rw = 1;
//		if (src->pages[i].user)    table->pages[i].user = 1;
//		if (src->pages[i].accessed)table->pages[i].accessed = 1;
//		if (src->pages[i].dirty)   table->pages[i].dirty = 1;
//		// Physically copy the data across. This function is in process.s.
//		copy_page_physical(src->pages[i].frame*0x1000, table->pages[i].frame*0x1000);
//	}
//	return table;
//}

page_t *vmm_create_page(uint32 v_addr, uint32 writeable, 
						uint32 user, page_directory_t *dir) {
	VMM_ALIGN_CHECK(v_addr);
	page_t *page;
	
	uint32 page_num = v_addr / VMM_PAGE_SIZE;
	
	uint32 table_indx = page_num / VMM_PAGES_PER_TABLE;
	uint32 page_indx = page_num % VMM_PAGES_PER_TABLE;
	
	if (!dir->tables[table_indx]) {
		uint32 addr;
		dir->tables[table_indx] = kmalloc_ex(sizeof(page_table_t), 1, &addr);
		dir->tables_phys[table_indx] = addr | 0x7; // Present, RW, US
	}
	
	page = &dir->tables[table_indx]->pages[page_indx];
	
	page->writeable = (writeable) ? 1 : 0;
	page->user = (user) ? 1 : 0;
	
	return page;
}

page_t *vmm_get_page(uint32 v_addr, page_directory_t *dir) {
	VMM_ALIGN_CHECK(v_addr);
	uint32 page_num = v_addr / VMM_PAGE_SIZE;
	
	uint32 table_indx = page_num / VMM_PAGES_PER_TABLE;
	uint32 page_indx = page_num % VMM_PAGES_PER_TABLE;
	
	if (dir->tables[table_indx])
		return &dir->tables[table_indx]->pages[page_indx];
	else
		return 0;
}

uint32 vmm_alloc_page(page_t *page) {
	uint32 addr = pmm_get_free_frame_addr();
	
	if (addr == (uint32)-1)
		PANIC("Not enough memory!");

	pmm_alloc_frame_addr(addr);
	page->p_addr = (addr >> 12);
	page->present = 1;
	
	return page->p_addr;
}

uint32 vmm_map_page(page_t *page, uint32 p_addr) {
	VMM_ALIGN_CHECK(p_addr);
	
	pmm_alloc_frame_addr(p_addr);
	page->p_addr = (p_addr >> 12);
	page->present = 1;
	
	return p_addr;
}

void vmm_free_page(page_t *page) {
	if (!page->p_addr)
		return;
	pmm_free_frame_addr(page->p_addr << 12);
	page->p_addr = 0x00;
	page->present = 0;
}

void vmm_set_current_page_directory(page_directory_t *page_directory) {
	current_page_dir = page_directory;
	switch_page_directory((uint32)(current_page_dir->phys_addr));
}

void page_fault_exeption_handler(exeption_info_t *exeption_info) {
	uint32 faulting_address;
	asm volatile("mov %%cr2, %0" : "=r" (faulting_address));
    
  	int present   = !(exeption_info->err_code & 0x1); // Page not present
    int rw = exeption_info->err_code & 0x2;           // Write operation?
    int us = exeption_info->err_code & 0x4;           // Processor was in user-mode?
    int reserved = exeption_info->err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
    //int id = exeption_info->err_code & 0x10;          // Caused by an instruction fetch?
	
	kprintf("Page fault! (");
    if (present) {kprintf("present ");}
    if (rw) {kprintf("read-only ");}
    if (us) {kprintf("user-mode ");}
    if (reserved) {kprintf("reserved ");}
    kprintf(") at 0x%x\n", faulting_address);
	
	uint32 cr3, cs;
	asm volatile("mov %%cs, %0" : "=r" (cs));
	asm volatile("mov %%cr3, %0" : "=r" (cr3));
	kprintf("CS = 0x%x\nDS = 0x%x\nCR3 = 0x%x\n", 
			cs, 
			exeption_info->ds,
			cr3);
	kprintf("Ex no = %x, Error code = %d\n", exeption_info->int_no, exeption_info->err_code);
	PANIC("Ooops! Page fault!");
}


