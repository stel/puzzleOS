/*
 *  paging.c
 *  puzzleOS
 *
 *  Created by Dmitry on 18.05.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#include "puzzle.h"

void enable_paging() {
	uint32 cr0;
	
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000;
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}

void disable_paging() {
	uint32 cr0;
	
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 &= 0x7FFFFFFF;
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}

void switch_page_directory(uint32 page_directory_addr) {
    asm volatile("mov %0, %%cr3":: "r"(page_directory_addr));
}
