/*
 *  idt.c
 *  puzzleOS
 *
 *  Created by Dmitry on 12.04.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#include "puzzle.h"
#include "interrupts.h"

#include "systables.h"
#include "types.h"
#include "string.h"

// Exeptions
extern void isr00();
extern void isr01();
extern void isr02();
extern void isr03();
extern void isr04();
extern void isr05();
extern void isr06();
extern void isr07();
extern void isr08();
extern void isr09();
extern void isr0A();
extern void isr0B();
extern void isr0C();
extern void isr0D();
extern void isr0E();
extern void isr0F();
extern void isr10();
extern void isr11();

// Reserved exeptions
extern void isr12_1F();

// IRQs
extern void irq00();
extern void irq01();
extern void irq02();
extern void irq03();
extern void irq04();
extern void irq05();
extern void irq06();
extern void irq07();
extern void irq08();
extern void irq09();
extern void irq0A();
extern void irq0B();
extern void irq0C();
extern void irq0D();
extern void irq0E();
extern void irq0F();

// Syscalls
extern void isr30();

extern p_interrupt_handler interrupt_handlers[];

idt_descriptor_t IDT[IDT_DESCRIPTORS_COUNT];
idt_reg_t IDTR;

void idt_init() {
	// Exeptions
	idt_set_descriptor(0x00, isr00, GDT_RING0_CODE_SELECTOR, IDT_PRESENT | IDT_INT_GATE | DPL0);
	idt_set_descriptor(0x01, isr01, GDT_RING0_CODE_SELECTOR, IDT_PRESENT | IDT_INT_GATE | DPL0);
	idt_set_descriptor(0x02, isr02, GDT_RING0_CODE_SELECTOR, IDT_PRESENT | IDT_INT_GATE | DPL0);
	idt_set_descriptor(0x03, isr03, GDT_RING0_CODE_SELECTOR, IDT_PRESENT | IDT_INT_GATE | DPL0);
	idt_set_descriptor(0x04, isr04, GDT_RING0_CODE_SELECTOR, IDT_PRESENT | IDT_INT_GATE | DPL0);
	idt_set_descriptor(0x05, isr05, GDT_RING0_CODE_SELECTOR, IDT_PRESENT | IDT_INT_GATE | DPL0);
	idt_set_descriptor(0x06, isr06, GDT_RING0_CODE_SELECTOR, IDT_PRESENT | IDT_INT_GATE | DPL0);
	idt_set_descriptor(0x07, isr07, GDT_RING0_CODE_SELECTOR, IDT_PRESENT | IDT_INT_GATE | DPL0);
	idt_set_descriptor(0x08, isr08, GDT_RING0_CODE_SELECTOR, IDT_PRESENT | IDT_INT_GATE | DPL0);
	idt_set_descriptor(0x09, isr09, GDT_RING0_CODE_SELECTOR, IDT_PRESENT | IDT_INT_GATE | DPL0);
	idt_set_descriptor(0x0A, isr0A, GDT_RING0_CODE_SELECTOR, IDT_PRESENT | IDT_INT_GATE | DPL0);
	idt_set_descriptor(0x0B, isr0B, GDT_RING0_CODE_SELECTOR, IDT_PRESENT | IDT_INT_GATE | DPL0);
	idt_set_descriptor(0x0C, isr0C, GDT_RING0_CODE_SELECTOR, IDT_PRESENT | IDT_INT_GATE | DPL0);
	idt_set_descriptor(0x0D, isr0D, GDT_RING0_CODE_SELECTOR, IDT_PRESENT | IDT_INT_GATE | DPL0);
	idt_set_descriptor(0x0E, isr0E, GDT_RING0_CODE_SELECTOR, IDT_PRESENT | IDT_INT_GATE | DPL0);
	idt_set_descriptor(0x0F, isr0F, GDT_RING0_CODE_SELECTOR, IDT_PRESENT | IDT_INT_GATE | DPL0);
	idt_set_descriptor(0x10, isr10, GDT_RING0_CODE_SELECTOR, IDT_PRESENT | IDT_INT_GATE | DPL0);
	idt_set_descriptor(0x11, isr11, GDT_RING0_CODE_SELECTOR, IDT_PRESENT | IDT_INT_GATE | DPL0);
	
	// Reserved exeptions
	int i;
	for (i = 0x12; i < 0x20; i++)
		idt_set_descriptor(i, isr12_1F, GDT_RING0_CODE_SELECTOR, IDT_PRESENT | IDT_INT_GATE | DPL0);
	
	// IRQs
	idt_set_descriptor(0x20, irq00, GDT_RING0_CODE_SELECTOR, IDT_PRESENT | IDT_INT_GATE | DPL0);
	idt_set_descriptor(0x21, irq01, GDT_RING0_CODE_SELECTOR, IDT_PRESENT | IDT_INT_GATE | DPL0);
	idt_set_descriptor(0x22, irq02, GDT_RING0_CODE_SELECTOR, IDT_PRESENT | IDT_INT_GATE | DPL0);
	idt_set_descriptor(0x23, irq03, GDT_RING0_CODE_SELECTOR, IDT_PRESENT | IDT_INT_GATE | DPL0);
	idt_set_descriptor(0x24, irq04, GDT_RING0_CODE_SELECTOR, IDT_PRESENT | IDT_INT_GATE | DPL0);
	idt_set_descriptor(0x25, irq05, GDT_RING0_CODE_SELECTOR, IDT_PRESENT | IDT_INT_GATE | DPL0);
	idt_set_descriptor(0x26, irq06, GDT_RING0_CODE_SELECTOR, IDT_PRESENT | IDT_INT_GATE | DPL0);
	idt_set_descriptor(0x27, irq07, GDT_RING0_CODE_SELECTOR, IDT_PRESENT | IDT_INT_GATE | DPL0);
	idt_set_descriptor(0x28, irq08, GDT_RING0_CODE_SELECTOR, IDT_PRESENT | IDT_INT_GATE | DPL0);
	idt_set_descriptor(0x29, irq09, GDT_RING0_CODE_SELECTOR, IDT_PRESENT | IDT_INT_GATE | DPL0);
	idt_set_descriptor(0x2A, irq0A, GDT_RING0_CODE_SELECTOR, IDT_PRESENT | IDT_INT_GATE | DPL0);
	idt_set_descriptor(0x2B, irq0B, GDT_RING0_CODE_SELECTOR, IDT_PRESENT | IDT_INT_GATE | DPL0);
	idt_set_descriptor(0x2C, irq0C, GDT_RING0_CODE_SELECTOR, IDT_PRESENT | IDT_INT_GATE | DPL0);
	idt_set_descriptor(0x2D, irq0D, GDT_RING0_CODE_SELECTOR, IDT_PRESENT | IDT_INT_GATE | DPL0);
	idt_set_descriptor(0x2E, irq0E, GDT_RING0_CODE_SELECTOR, IDT_PRESENT | IDT_INT_GATE | DPL0);
	idt_set_descriptor(0x2F, irq0F, GDT_RING0_CODE_SELECTOR, IDT_PRESENT | IDT_INT_GATE | DPL0);
	
	// Syscalls
	idt_set_descriptor(0x30, isr30, GDT_RING0_CODE_SELECTOR, IDT_PRESENT | IDT_INT_GATE | DPL3);
	
	// Setup IDTR
	IDTR.base = (uint32)IDT;
	IDTR.limit = (sizeof(idt_descriptor_t) * IDT_DESCRIPTORS_COUNT) - 1;
	
	idtr_set(&IDTR);
}

idt_descriptor_t *idt_set_descriptor(uint8 num, i_handler handler, uint16 selector, uint8 attr) {
	if (num >= IDT_DESCRIPTORS_COUNT)
		PANIC("Wrong IDT descriptor number!");
	
	IDT[num].gdt_sel = selector;
	
	IDT[num].offset_lo = ((uint32)handler & 0xFFFF);
	IDT[num].offset_hi = (((uint32)handler >> 16) & 0xFFFF);
	
	IDT[num].zero = 0;
	IDT[num].attr = attr;// | 0x80; // TYPE | PRESENT (present = 1)
	
	return &IDT[num];
}

void idtr_set(dt_reg_t *idtr) {
	asm("lidt 0(,%0,)"::"a"(idtr));
}

void isr(interrupt_info_t *interrupt_info) {
	interrupt_handlers[interrupt_info->int_no](interrupt_info);
}
