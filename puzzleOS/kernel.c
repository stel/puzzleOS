/*
 *  kernel.c
 *  puzzleOS
 *
 *  Created by Dmitry on 26.02.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#include "puzzle.h"
#include "multiboot.h"
#include "vga.h"
#include "console.h"
#include "stdarg.h"
#include "proc.h"
#include "vmm.h"
#include "fs.h"

#include "rd.h"

#include "atapi.h"

extern uint32 placement_address;

void kmain(multiboot_header_t *multiboot_header) {
	
	kprintf("Puzzle OS %d.%d\n", PUZZLE_RELEASE, PUZZLE_VERSION);
	
	uint32 rd_location = *((uint32*)multiboot_header->mods_addr);
	uint32 rd_end = *(uint32*)(multiboot_header->mods_addr+4);
	
	// kernel size = ramdisk end
	placement_address = rd_end;
	
	vmm_init();
	
	
	kheap = create_heap(KHEAP_START, KHEAP_START+KHEAP_INITIAL_SIZE, 0xCFFFF000, 0, 0);
	
	kprintf("Mounting root file system...\n");
	fs_root = rd_init(rd_location);
	
	proc_init();
	
	//atapi_init();
	
	proc_t *shell;
	
	int i;
	for (i = 0; i < CONSOLES_COUNT; i++) {
		shell = exec("psh.bin");
		shell->console = consoles + i;
	}
}
