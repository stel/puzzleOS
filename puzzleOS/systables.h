/*
 *  systables.h
 *  puzzleOS
 *
 *  Created by Dmitry on 18.04.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#ifndef _SYSTABLES_H_
#define _SYSTABLES_H_

#define RPL0	0x00
#define RPL1	0x01
#define RPL2	0x02
#define RPL3	0x03

#define DPL0	0x00	// 0x00 << 5
#define DPL1	0x20	// 0x01 << 5
#define DPL2	0x40	// 0x02 << 5
#define DPL3	0x60	// 0x03 << 5

#define GDT_DESCRIPTORS_COUNT 6
#define IDT_DESCRIPTORS_COUNT 49

#define GDT_RING0_CODE_SELECTOR	0x08
#define GDT_RING0_DATA_SELECTOR	0x10
#define GDT_RING0_STACK_SELECTOR GDT_RING0_DATA_SELECTOR

#define GDT_RING3_CODE_SELECTOR	0x18
#define GDT_RING3_DATA_SELECTOR	0x20
#define GDT_RING3_STACK_SELECTOR GDT_RING3_DATA_SELECTOR
#define GDT_TSS_SELECTOR		0x28

#define GDT_PRESENT			0x90	// present=1 (bit7), bit4=1
#define GDT_EXECUTABLE		0x08
#define GDT_DIRECTION_TOP	0x04
#define GDT_RW				0x02

#define GDT_GRANULARITY

#define IDT_PRESENT		0x80

#define IDT_TASK_GATE	0x05
#define IDT_INT_GATE	0x0E
#define IDT_TRAP_GATE	0x0F

typedef struct descriptors_table_reg {
	uint16	limit;
	uint32	base;
} __attribute__((packed)) dt_reg_t;

typedef dt_reg_t gdt_reg_t;
typedef dt_reg_t idt_reg_t;

typedef struct gdt_descriptor {
	uint16	limit_lo;
	uint16	base_lo;
	uint8	base_mid;
	uint8	access;
	uint8	flags		: 4;
	uint8	limit_hi	: 4;
	//uint8	granularity;
	uint8	base_hi;
	
} __attribute__((packed)) gdt_descriptor_t;

typedef struct idt_descriptor {
	uint16	offset_lo;
	uint16	gdt_sel;
	uint8	zero;
	uint8	attr;
	uint16	offset_hi;
} __attribute__((packed)) idt_descriptor_t;

typedef struct tss {
	int32	back_link;
	int32	esp0, ss0;		
	int32	esp1, ss1;		
	int32	esp2, ss2;		
	int32	cr3;
	int32	eip;
	int32	eflags;
	int32	eax, ecx, edx, ebx, esp, ebp, esi, edi;
	int32	es, cs, ss, ds, fs, gs;		
	int32	ldt;		
	int32	io_map;
} __attribute__((packed)) tss_t;

// ISR pointer
typedef void (*i_handler)();

gdt_descriptor_t *gdt_set_descriptor(uint8 num, uint32 base, uint32 limit, 
									 uint8 access, uint8 gran);
void gdtr_set(dt_reg_t *gdtr);

idt_descriptor_t *idt_set_descriptor(uint8 num, i_handler handler, uint16 selector, uint8 attr);
void idtr_set(dt_reg_t *idtr);

#endif
