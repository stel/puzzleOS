/*
 *  console.h
 *  puzzleOS
 *
 *  Created by Dmitry on 07.04.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include "list.h"

#define CONSOLE_INBUFFER_SIZE 32

#define CONSOLES_COUNT 10

#define	F1	0x3B
#define	F2	0x3C
#define	F3	0x3D
#define	F4	0x3E
#define	F5	0x3F
#define	F6	0x40
#define	F7	0x41
#define	F8	0x42
#define	F9	0x43
#define	F10	0x44

typedef struct {
	char buffer[CONSOLE_INBUFFER_SIZE];
	uint8 pos;
	uint16 size;
} p_console_inbuffer;

typedef struct console {
	uint8 num;
	struct {
		uint8 x;
		uint8 y;
	} cursor;
	uint8 attr;
	uint16 *frame_buffer_addr;
	p_console_inbuffer inbuffer;
	
	list_t *io_queue;
} p_console;

void set_console(uint8 num);
void console_init();
void console_write(p_console *console, char c);
void console_set_attr(p_console *console, unsigned char attr);
char console_read(p_console *console);

extern p_console *current_console;
extern p_console consoles[];

#endif
