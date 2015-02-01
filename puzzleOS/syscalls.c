/*
 *  syscalls.c
 *  puzzleOS
 *
 *  Created by Dmitry on 17.05.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#include "puzzle.h"
#include "syscalls.h"
#include "interrupts.h"
#include "proc.h"

int reshedule = 0;

void syscalls_init() {
	//set_interrupt_handler(0x30, syscalls_interrupt_handler);
	enable_interrupt(0x30);
}

int syscalls_interrupt_handler(interrupt_info_t *interrupt_info) {
	int syscall_num = interrupt_info->eax;
	int res = 0;
	
	reshedule = 0;
	
	current_proc->context = (context_t *)interrupt_info;
	
	res = syscalls_table[syscall_num](interrupt_info->ebx, 
									  interrupt_info->ecx, 
									  interrupt_info->edx);
	
	current_proc->context->eax = res;
	
	return reshedule;
}

