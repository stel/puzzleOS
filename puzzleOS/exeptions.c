/*
 *  exeptions.c
 *  puzzleOS
 *
 *  Created by Dmitry on 13.04.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#include "puzzle.h"
#include "interrupts.h"

#include "exeptions.h"

static const char *exeption_messages[] = {
	"Divide Error",
	"Debug Exceptions",
	"Non Maskable Interrupt ",
	"Breakpoint",
	"Overflow",
	"Bounds Check",
	"Invalid Opcode",
	"Coprocessor Not Available",
	"Double Fault",
	"Coprocessor Segment Overrun",
	"Invalid TSS",
	"Segment Not Present",
	"Stack Exception",
	"General Protection Fault",
	"Page Fault",
	"Intel reserved",
	"Coprocessor Error",
	"Alignment Check",
	"Machine Check"
};

static void exeption_handler(exeption_info_t *exeption_info);

void exeptions_init() {
	int i = 0;
	for (i = 0; i < EXEPTIONS_COUNT; i++)
		set_interrupt_handler(i, &exeption_handler);
}

//static void page_fault(p_exeption_info *exeption_info);

void exeption_handler(exeption_info_t *exeption_info) {
	output_interrupt_info(exeption_info);
	
	switch (exeption_info->int_no) {
	//	case 0x0E:
//			page_fault(exeption_info);
//			break;
//			
		default:
			kprintf("\nOh no! %s! (int #%d)\n", exeption_messages[exeption_info->int_no], 
					exeption_info->int_no);
			PANIC ("Unhandled exeption");
	}
}


//void page_fault(p_exeption_info *exeption_info) {
//    // The faulting address is stored in the CR2 register.
//    uint32 faulting_address;
//    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));
//    
//   // // The error code gives us details of what happened.
//	int present   = !(exeption_info->err_code & 0x1); // Page not present
//    int rw = exeption_info->err_code & 0x2;           // Write operation?
//    int us = exeption_info->err_code & 0x4;           // Processor was in user-mode?
//    int reserved = exeption_info->err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
//    int id = exeption_info->err_code & 0x10;          // Caused by an instruction fetch?
//	
//    // Output an error message.
//   // monitor_write("Page fault! ( ");
////    if (present) {monitor_write("present ");}
////    if (rw) {monitor_write("read-only ");}
////    if (us) {monitor_write("user-mode ");}
////    if (reserved) {monitor_write("reserved ");}
////    monitor_write(") at 0x");
////    monitor_write_hex(faulting_address);
////    monitor_write("\n");
//	PANIC("Page fault");
//}
