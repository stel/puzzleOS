/*
 *  gdt.c
 *  puzzleOS
 *
 *  Created by Dmitry on 26.04.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#include "puzzle.h"
#include "systables.h"

gdt_descriptor_t GDT[GDT_DESCRIPTORS_COUNT];
gdt_reg_t GDTR;

void gdt_init() {
	// Null
	gdt_set_descriptor(0, 0, 0, 0, 0);
	
	// Ring0 code
	gdt_set_descriptor(1, 0, 0xFFFFFFFF, GDT_PRESENT | GDT_EXECUTABLE | GDT_RW, 0xCF);
	// Ring0 data
	gdt_set_descriptor(2, 0, 0xFFFFFFFF, GDT_PRESENT | GDT_RW, 0xCF);
	
	// Ring3 code
	gdt_set_descriptor(3, 0, 0xFFFFFFFF, GDT_PRESENT | GDT_EXECUTABLE | GDT_RW | DPL3, 0xCF);
	// Ring3 data
	gdt_set_descriptor(4, 0, 0xFFFFFFFF, GDT_PRESENT | GDT_RW | DPL3, 0xCF);
	
	// TSS
	gdt_set_descriptor(5, 0, 0, 0, 0);
	
	// Setup GDTR
	GDTR.base = (uint32)GDT;
	GDTR.limit = (sizeof(gdt_descriptor_t) * GDT_DESCRIPTORS_COUNT) - 1;
	
	gdtr_set(&GDTR);
}

gdt_descriptor_t *gdt_set_descriptor(uint8 num, uint32 base, uint32 limit, 
									 uint8 access, uint8 flags) {
	if (num >= GDT_DESCRIPTORS_COUNT)
		PANIC("Wrong GDT descriptor number!");
	
	GDT[num].base_lo = (base & 0xFFFF);
	GDT[num].base_mid = (base >> 16) & 0xFF;
	GDT[num].base_hi = (base >> 24) & 0xFF;
	
	GDT[num].limit_lo = (limit & 0xFFFF);
	GDT[num].limit_hi = ((limit >> 16) & 0x0F);
	
	GDT[num].access = access;
	GDT[num].flags = flags;
	
	return &GDT[num];
}

void gdtr_set(dt_reg_t *gdtr) {
	asm("lgdt 0(,%0,)"::"a"(gdtr));
}
