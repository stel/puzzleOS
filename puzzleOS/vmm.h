/*
 *  vmm.h
 *  puzzleOS
 *
 *  Created by Dmitry on 13.05.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#ifndef _VMM_H_
#define _VMM_H_

#define VMM_PAGE_SIZE 0x1000 // 4Kb
#define VMM_PAGES_PER_TABLE 1024
#define VMM_TABLES_PER_DIRECTORY 1024

#define PAGE_WRITEABLE 1
#define PAGE_READEADBLE 0
#define PAGE_USER 1
#define PAGE_SUPERVISOR 0

typedef struct page {
    uint32 present		: 1;
    uint32 writeable	: 1;	// 0 - read-only, 1 - readwrite
    uint32 user			: 1;	// 0 - supervisor, 1 - user
	uint32 rsvd			: 2;	// reserved
    uint32 accessed		: 1;
    uint32 dirty		: 1;
    uint32 unused		: 5;
    uint32 p_addr		: 20;	// phys_addr % 0x1000
} page_t;

typedef struct page_table {
    page_t pages[1024];
} page_table_t;

typedef struct page_directory {
    page_table_t *tables[1024];
    uint32 tables_phys[1024];
    uint32 phys_addr;
} page_directory_t;

void vmm_init();
page_directory_t *vmm_create_page_directory();
page_directory_t *vmm_copy_page_directory(page_directory_t *dir, page_directory_t *src);
void vmm_set_current_page_directory(page_directory_t *page_directory);


page_t *vmm_create_page(uint32 v_addr, uint32 writeable, 
						uint32 user, page_directory_t *dir);
page_t *vmm_get_page(uint32 v_addr, page_directory_t *dir);

uint32 vmm_alloc_page(page_t *page);
uint32 vmm_map_page(page_t *page, uint32 p_addr);
void vmm_free_page(page_t *page);

extern page_directory_t *kernel_page_dir;
extern page_directory_t *current_page_dir;

#endif
