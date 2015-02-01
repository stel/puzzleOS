/*
 *  exec.c
 *  puzzleOS
 *
 *  Created by Dmitry on 03.06.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#include "puzzle.h"
#include "proc.h"
#include "fs.h"
#include "vmm.h"

proc_t *exec(char *file_name) {
	uint32 *proc_image;
	uint8 *ptr;
	
	fs_node_t *fsnode = fs_finddir(fs_root, file_name);
	
	if (fsnode)	{
		uint32 phys;
		proc_image = kmalloc_ex(fsnode->length, 1, &phys);
		//kprintf("PROG SIZE = %x\n", fsnode->length);
		
		ptr = (uint8 *)proc_image;
		
		fs_read(fsnode, 0, fsnode->length, (uint8 *)proc_image);
		
		proc_t *proc = create_proc((uint32)proc_image);
		proc->addr = proc_image;
		
		int i;
		for (i = 0; i < 5; i++) 
			vmm_map_page(vmm_create_page(0x400000 + i * VMM_PAGE_SIZE, 1, 1, proc->page_dir), phys + i * VMM_PAGE_SIZE);
		
		strcpy(proc->name, file_name);
		return proc;
	}
	return 0;
}
