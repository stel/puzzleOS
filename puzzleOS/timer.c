/*
 *  timer.c
 *  puzzleOS
 *
 *  Created by Dmitry on 25.04.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#include "puzzle.h"
#include "interrupts.h"


static void timer_interrupt_handler(interrupt_info_t *interrupt_info);

//static int ticks = 0;

void timer_init() {
	set_interrupt_handler(0x20, timer_interrupt_handler);
	enable_interrupt(0x20);
}

void print_ii(interrupt_info_t *ii) {
	kprintf("II\nGS=0x%x, FS=0x%x, ES=0x%x, DS=0x%x\nEDI=0x%x, ESI=0x%x, EBP=0x%x, ESP=0x%x\nEBX=0x%x, EDX=0x%x, ECX=0x%x, EAX=0x%x\nINT_NO=0x%x, ERR_CODE=0x%x, EIP=0x%x, CS=0x%x\nEFLAGS=0x%x\n",
			ii->gs, ii->fs, ii->es, ii->ds,     
			ii->edi, ii->esi, ii->ebp, ii->esp, ii->ebx, ii->edx, ii->ecx, ii->eax,  
			ii->int_no, ii->err_code,  
			ii->eip, ii->cs, ii->eflags);
}


void timer_interrupt_handler(interrupt_info_t *interrupt_info) {
//	ticks++;
//	if (ticks % 18 == 0)
//	{
//		p("----------------------------- TIMER! --------------------------\n");
//	//	print_ii(interrupt_info);
//	}
}
