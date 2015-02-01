/*
 *  vga.c
 *  puzzleOS
 *
 *  Created by Dmitry on 08.04.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#include "puzzle.h"
#include "vga.h"
#include "ioports.h"

static uint16 frame_buffer_addr;

void vga_init() {
	frame_buffer_addr = (uint16)VGA_FRAME_BUFFER_START;
}

void vga_set_frame_buffer_addr(uint32 addr) {
	
	frame_buffer_addr = (addr - VGA_FRAME_BUFFER_START) / 2;
	
	outportb(0x3D4, 12);
	outportb(0x3D5, frame_buffer_addr >> 8);
	outportb(0x3D4, 13);
	outportb(0x3D5, frame_buffer_addr);
}

void vga_set_cursor_pos(int x, int y) {
	int pos;
	
	pos = y * VGA_SCREEN_COLS + x + frame_buffer_addr;
	
	outportb(0x3D4, 0xE);
	outportb(0x3D5, pos >> 8);
	outportb(0x3D4, 0xF);
	outportb(0x3D5, pos & 0xFF);
}

//void vga_write(int x, int y, char c, char attr) {
//	*((uint32 *)frame_buffer_addr + y * VGA_SCREEN_ROWS + x) = (attr << 8) + c;
//}
