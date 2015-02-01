/*
 *  tss.c
 *  puzzleOS
 *
 *  Created by Dmitry on 16.05.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#include "puzzle.h"
#include "systables.h"

tss_t *tss = 0;

static void tr_set(uint32 tr);

void tss_init() {
	tss = kmalloc(sizeof(tss_t));
	
	tss->esp0	  = 1024;
	tss->ss0	  = GDT_RING0_STACK_SELECTOR;	// Ring0 SS
	
	//tss->eip	  = eip;
	//tss->eflags	  = 0;//0x202;	//IF flag
	
	//tss->cr3 = 0x10f000;
	
	//tss->esp	  = 0;
	
	tss->cs = GDT_RING0_CODE_SELECTOR | RPL3;
	tss->ds = tss->es = tss->fs = tss->gs = GDT_RING0_DATA_SELECTOR | RPL3;
	tss->ss = GDT_RING0_STACK_SELECTOR | RPL3;
	
	gdt_set_descriptor(5, (uint32)tss, sizeof(tss_t)-1, 0xE9, 0x00);
	tr_set(GDT_TSS_SELECTOR | RPL3);
}

void tr_set(uint32 tr) {
	asm("ltr %%ax"::"a"(tr));
}
