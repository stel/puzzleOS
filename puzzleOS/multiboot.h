/*
 *  multiboot.h
 *  puzzleOS
 *
 *  Created by Dmitry on 02.06.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#ifndef _MULTIBOOT_H_
#define _MULTIBOOT_H_

struct multiboot_header {
	uint32 flags;
	uint32 mem_lower;
	uint32 mem_upper;
	uint32 boot_device;
	uint32 cmdline;
	uint32 mods_count;
	uint32 mods_addr;
	uint32 num;
	uint32 size;
	uint32 addr;
	uint32 shndx;
	uint32 mmap_length;
	uint32 mmap_addr;
	uint32 drives_length;
	uint32 drives_addr;
	uint32 config_table;
	uint32 boot_loader_name;
	uint32 apm_table;
	uint32 vbe_control_info;
	uint32 vbe_mode_info;
	uint32 vbe_mode;
	uint32 vbe_interface_seg;
	uint32 vbe_interface_off;
	uint32 vbe_interface_len;
}  __attribute__((packed));

typedef struct multiboot_header multiboot_header_t;

#endif
