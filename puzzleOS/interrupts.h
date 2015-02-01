/*
 *  interrupts.h
 *  puzzleOS
 *
 *  Created by Dmitry on 19.04.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#ifndef _INTERRUPTS_H_
#define _INTERRUPTS_H_

#define EXEPTIONS_COUNT 32
#define	IRQ_START 32
#define IRQ_COUNT 16
#define INTERRUPTS_COUNT EXEPTIONS_COUNT + IRQ_COUNT + 1 // + Syscall

typedef struct {
	int32 gs, fs, es, ds;     
    int32 edi, esi, ebp, esp, ebx, edx, ecx, eax;  
    int32 int_no, err_code;  
	int32 eip, cs, eflags;
} interrupt_info_t;

typedef void (*p_interrupt_handler)(interrupt_info_t *interrupt_info);

void set_interrupt_handler(uint32 num, p_interrupt_handler interrupt_handler);

void enable_interrupt(uint32 num);
void disable_interrupt(uint32 num);

void output_interrupt_info(interrupt_info_t *interrupt_info);

#endif
