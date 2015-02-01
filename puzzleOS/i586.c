/*
 *  i586.c
 *  puzzleOS
 *
 *  Created by Dmitry on 19.04.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#include "puzzle.h"
#include "i586.h"

void arch_init() {
	gdt_init();
	tss_init();
	pic_init();
	idt_init();
}

void cli() {
	asm("cli");
}

void sti() {
	asm("sti");
}


