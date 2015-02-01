/*
 *  kbd.c
 *  puzzleOS
 *
 *  Created by Dmitry on 08.04.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#include "puzzle.h"
#include "interrupts.h"
#include "kbd.h"
#include "ioports.h"

uint8 kbd_state;

void kbd_release();
static void kbd_process_specials(uint8 scan);

extern void process_key(uint8 key);

void kbd_interrupt_handler(interrupt_info_t *interrupt_info) {
	uint8 scan;
	
	scan = inportb(0x60);
	
	kbd_process_specials(scan);
	
	process_key(scan);
	
	kbd_release();
}

void kbd_process_specials(uint8 scan) {
	
	if ((kbd_state | CTRL) && (kbd_state | ALT) && scan == 0x53)
		outportb(0x64, 0xFE);	// reboot
		
	switch (scan) {
		case 0x1D:	// CTRL
			kbd_state |= CTRL;
			break;
			
		case 0x1D + 0x80:	// CTRL
			kbd_state ^= CTRL;
			break;
			
		case 0x38:	// ALT
			kbd_state |= ALT;
			break;
			
		case 0x38 + 0x80:	// ALT
			kbd_state ^= ALT;
			break;
			
		case 0x36:
		case 0x2A:	// SHIFT
			kbd_state |= SHIFT;
			break;
			
		case 0x36 + 0x80:
		case 0x2A + 0x80:	// SHIFT
			kbd_state ^= SHIFT;
			break;
			
		default:
			break;
	}
}

void kbd_release() {
	uint8 b;
	
	b = inportb(0x61);
	b |= 1;
	outportb(0x61, b);
}

void kbd_init() {
	kbd_state = 0;
	
	set_interrupt_handler(0x21, kbd_interrupt_handler);
	enable_interrupt(0x21);
}
