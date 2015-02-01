/*
 *  kbd.h
 *  puzzleOS
 *
 *  Created by Dmitry on 08.04.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#ifndef _KBD_H_
#define _KBD_H_

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

#define CTRL		0x01
#define ALT			0x02
#define	SHIFT		0x04
#define	CAPSLOCK	0x08

#define KBD_BUFFER_SIZE 32

typedef struct {
	uint8 *addr;
	uint32 size;
	uint32 pos;
} p_kbd_buffer;

void kbd_init();
void kbd_interrupt();

extern uint8 kbd_state;

#endif
