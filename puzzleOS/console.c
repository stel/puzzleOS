/*
 *  console.c
 *  puzzleOS
 *
 *  Created by Dmitry on 07.04.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */


#include "puzzle.h"
#include "vga.h"
#include "kbd.h"
#include "keymap.h"
#include "list.h"
#include "console.h"
#include "proc.h"


#define IOPORT 0x3D4

p_console consoles[CONSOLES_COUNT];
p_console *current_console;

//extern io_queue_t *io_queue;

static void console_update_cursor();

static void console_update_cursor() {
	vga_set_cursor_pos(current_console->cursor.x, current_console->cursor.y);
}

void console_gotoxy(p_console *console, int x, int y) {
	console->cursor.x = x;
	console->cursor.y = y;
	console_update_cursor(console);
}

void console_scroll_up(p_console *console) {
	memcpy(console->frame_buffer_addr, 
		   console->frame_buffer_addr+VGA_SCREEN_COLS,
		   VGA_SCREEN_COLS * (VGA_SCREEN_ROWS - 1) * 2);
	
	memsetw(console->frame_buffer_addr + VGA_SCREEN_COLS * (VGA_SCREEN_ROWS - 1), 
			0x20 | (console->attr << 8), 
			VGA_SCREEN_COLS);
}

void console_scroll_down(p_console *console) {
	
}

void console_clear(p_console *console) {
	memsetw(console->frame_buffer_addr, 0x20 | (console->attr << 8), 
			VGA_SCREEN_ROWS * VGA_SCREEN_COLS);
}

void console_write(p_console *console, char c) {
	uint16 *current_char;
	
	current_char = console->frame_buffer_addr 
		+ console->cursor.y * VGA_SCREEN_COLS 
		+ console->cursor.x;
	
	switch (c) {
		case 0x00:
			return;
			break;
			
		case 0x1B:	// esc
			return;
			break;
			
		case 0x08:	// backspace
			if(console->cursor.x > 0)
			{
				console->cursor.x--;
				*(--current_char) = (' ' | (console->attr << 8));
			}			
			break;

		case '\t':
			console->cursor.x = (console->cursor.x + 8) & ~(8 - 1);
			break;
			
		case '\r':
			console->cursor.x = 0;
			break;

		case '\n':
			console->cursor.x = 0;
			if (console->cursor.y < VGA_SCREEN_ROWS - 1)
				console->cursor.y++;
			else
				console_scroll_up(console);
			break;

		default:
			if (c >= ' ') {
				*current_char = (c | (console->attr << 8));
				console->cursor.x++;
			}	
			break;
	}
	
	if (console->cursor.x >= VGA_SCREEN_COLS) {
		console->cursor.x = 0;
		if (console->cursor.y < VGA_SCREEN_ROWS - 1)
			console->cursor.y++;
		else
			console_scroll_up(console);
	}
	
	if (console == current_console)
		console_update_cursor();
}

void console_set_attr(p_console *console, uint8 attr) {
	console->attr = attr;
}

void process_key(uint8 key) {
	char ascii;
	
	if (current_console->inbuffer.pos >= 255 && key != 0x0E) {
		return;
	}
		
	switch (key) {
		case F1:
		case F2:
		case F3:
		case F4:
		case F5:
		case F6:
		case F7:
		case F8:
		case F9:
		case F10:
			set_console(key - 0x3B);
			break;
		//case 0x0E:
//			if (inbuffer->pos > 0) {
//				inbuffer->pos--;
//				
//				console_write(current_console, 0x08);
//			}
//			break;
		default:
			if (key < 0x3A) {
				
				if (kbd_state & SHIFT)
					ascii = scancodes_shift[key];
				else
					ascii = scancodes[key];
				
				if (current_console->io_queue->items_count > 0) {
					
					
					if (ascii == 8) {
						if (current_console->inbuffer.pos > 0) {
							current_console->inbuffer.pos--;
							console_write(current_console, ascii);
						} else ascii = 0;
					} else if (ascii == '\n') {
						current_console->inbuffer.pos = 0;
						console_write(current_console, ascii);
					} else {
						current_console->inbuffer.pos++;
						console_write(current_console, ascii);
					}
					
					//char c = console_read(current_console);
					char c = ascii;
					proc_t *proc;
					if (c) {
						while (current_console->io_queue->items_count > 0) {
							proc = list_pop_front(current_console->io_queue);
							proc->context->eax = c;
							proc->state = PS_RUNABLE;
						}
					}
				}
			}
			break;
	}
}

char console_read(p_console *console) {
	p_console_inbuffer *inbuffer = &console->inbuffer;
	
	if (inbuffer->pos > 0)
		return inbuffer->buffer[--inbuffer->pos];
	
	return 0;
}

void set_console(uint8 num) {
	if (num >= CONSOLES_COUNT)
		return;
	
	current_console = consoles + num;
	
	vga_set_frame_buffer_addr((uint32)current_console->frame_buffer_addr);
	
	console_update_cursor(current_console);
}

void console_init() {
	int i;
	
	vga_init();
	kbd_init();
	
	for (i = 0; i < CONSOLES_COUNT; i++) {
		current_console = consoles + i;
		current_console->frame_buffer_addr = (uint16 *)VGA_FRAME_BUFFER_START +
									(VGA_SCREEN_ROWS * VGA_SCREEN_COLS) * i;
		current_console->attr = 0x07;
		current_console->num = i;
		current_console->cursor.x = 0;
		current_console->cursor.y = 0;
		current_console->inbuffer.pos = 0;
		
		current_console->io_queue = kmalloc(sizeof(list_t));
		
		console_clear(current_console);
		kprintf("con #%d\n", current_console->num);
	}
	
	set_console(0);
}

// for kprintf
void kputch(char c) {
	console_write(current_console, c);
}
