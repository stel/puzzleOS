/*
 *  pic.c
 *  puzzleOS
 *
 *  Created by Dmitry on 19.04.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#include "puzzle.h"
#include "interrupts.h"
#include "ioports.h"

#define PRIMARY_CTRL	0x20	// Порт первичного контроллера прерываний
#define PRIMARY_MASK	0x21

#define SECONDARY_CTRL	0xA0	// Порт вторичного контроллера прерываний
#define SECONDARY_MASK	0xA1

extern p_interrupt_handler interrupt_handlers[];

void pic_init() {
	outportb(PRIMARY_CTRL, 0x11);
	outportb(SECONDARY_CTRL, 0x11);
	
	outportb(PRIMARY_MASK, 0x20);
	outportb(SECONDARY_MASK, 0x28);
	
	outportb(PRIMARY_MASK, 0x04);
	outportb(SECONDARY_MASK, 0x02);
	
	outportb(PRIMARY_MASK, 0x0D);
	outportb(SECONDARY_MASK, 0x09);
	
	// Запретить все прерывание
	outportb(PRIMARY_MASK, 0xFF);
}

void pic_irq(interrupt_info_t *interrupt_info) {
	
	if (interrupt_info->int_no > 0x08)
		outportb(SECONDARY_CTRL, 0x20);
	outportb(PRIMARY_CTRL, 0x20);
	
	interrupt_info->int_no += 0x20;
	
	interrupt_handlers[interrupt_info->int_no](interrupt_info);
}

void pic_enable_irq(uint32 irq) {
	uint8 mask;
	
	if (irq < 0x28) {
		mask = inportb(PRIMARY_MASK);
		mask &= ~(1 << irq);
		outportb(PRIMARY_MASK, mask);
	} else {
		mask = inportb(SECONDARY_MASK);
		mask &= ~(1 << irq);
		outportb(SECONDARY_MASK, mask);
	}
}

void pic_disable_irq(uint32 irq) {
	uint8 mask;
	
	if (irq < 0x28) {
		mask = inportb(PRIMARY_MASK);
		mask |= 1 << irq;
		outportb(PRIMARY_MASK, mask);
	} else {
		mask = inportb(SECONDARY_MASK);
		mask |= 1 << irq;
		outportb(SECONDARY_MASK, mask);
	}
}

