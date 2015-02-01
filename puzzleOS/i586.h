/*
 *  i586.h
 *  puzzleOS
 *
 *  Created by Dmitry on 19.04.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#ifndef _I586_H_
#define _I586_H_

#include "systables.h"

void arch_init();

void gdt_init();
void idt_init();
void tss_init();

void pic_init();
void pic_enable_irq(uint32 irq);
void pic_disable_irq(uint32 irq);

void enable_paging();
void disable_paging();
void switch_page_directory(uint32 page_directory_addr);

void cli();
void sti();

extern tss_t *tss;
#endif
