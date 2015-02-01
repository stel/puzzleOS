/*
 *  vga.h
 *  puzzleOS
 *
 *  Created by Dmitry on 08.04.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#ifndef _VGA_H_
#define _VGA_H_

#define VGA_FRAME_BUFFER_START 0xB8000
#define VGA_SCREEN_ROWS 25
#define VGA_SCREEN_COLS 80

void vga_init();
void vga_set_frame_buffer_addr(uint32 addr);
void vga_set_cursor_pos(int x, int y);
//void vga_write(int x, int y, char c, char attr);


#endif
