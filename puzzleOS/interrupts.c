/*
 *  interrupts.c
 *  puzzleOS
 *
 *  Created by Dmitry on 19.04.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#include "puzzle.h"
#include "interrupts.h"
#include "exeptions.h"

p_interrupt_handler interrupt_handlers[INTERRUPTS_COUNT];

static void dummy(interrupt_info_t *interrupt_info);

void interrupts_init() {
	int i;
	
	exeptions_init();
	
	for (i = EXEPTIONS_COUNT; i < INTERRUPTS_COUNT; i++) {
		set_interrupt_handler(i, dummy);
	}
	
	enable_interrupt(0x20);
}

void dummy(interrupt_info_t *interrupt_info) {
	kprintf("Interrupt #%d!\n", interrupt_info->int_no);
	PANIC("Unhandled interrupt!");
}

void set_interrupt_handler(uint32 num, p_interrupt_handler interrupt_handler) {
	if (num >= INTERRUPTS_COUNT) {
		kprintf("Trying to assign a wrong interrupt (#%d)!\n", num);
		PANIC("Wrong interrupt number!");
	}
	interrupt_handlers[num] = interrupt_handler;
}

void enable_interrupt(uint32 num) {
	if (num >= IRQ_START)
		pic_enable_irq(num - IRQ_START);
}

void disable_interrupt(uint32 num) {
	if (num >= IRQ_START)
		pic_disable_irq(num - IRQ_START);
}

void output_interrupt_info(interrupt_info_t *interrupt_info) {
	kprintf("Interrupt number: %d\n", interrupt_info->int_no);
	kprintf("Error code: 0x%x\n", interrupt_info->err_code);
	kprintf("Instruction address:\n");
	kprintf("CS = 0x%x, EIP = 0x%x\n", interrupt_info->cs, interrupt_info->eip);
	kprintf("Stack:\n");
	kprintf("ESP = 0x%x, EBP = 0x%x\n", interrupt_info->esp, interrupt_info->ebp);
	kprintf("Segment registers:\n");
	kprintf("CS = 0x%x\tES = 0x%x\n", interrupt_info->cs, interrupt_info->es);
	kprintf("DS = 0x%x\tFS = 0x%x\n", interrupt_info->ds, interrupt_info->fs);
	kprintf("SS = 0x%x\tGS = 0x%x\n", interrupt_info->ds, interrupt_info->gs);
	kprintf("Common registers:\n");
	kprintf("EAX = 0x%x\tEDX = 0x%x\n", interrupt_info->eax, interrupt_info->edx);
	kprintf("EBX = 0x%x\tEDI = 0x%x\n", interrupt_info->ebx, interrupt_info->edi);
	kprintf("ECX = 0x%x\tESI = 0x%x\n", interrupt_info->ecx, interrupt_info->esi);
}
