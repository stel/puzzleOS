/*
 *  ioports.c
 *  puzzleOS
 *
 *  Created by Dmitry on 08.04.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */
#include "puzzle.h"
#include "ioports.h"

uint8 inportb(uint16 port) {
	uint8 value;
	asm volatile("inb %%dx, %%al":"=a" (value):"d" (port));
	return value;
}

uint16 inportw(uint16 port) {
	uint16 value;
	asm volatile("inw %%dx, %%ax":"=a" (value):"d" (port));
	return value;
}

uint32 inportl(uint16 port) {
	uint32 value;
	asm volatile("in %%dx, %%eax":"=a" (value):"d" (port));
	return value;
}

void outportb(uint16 port, uint8 value) {
	asm volatile("outb %%al, %%dx"::"d" (port), "a" (value));
}

void outportw(uint16 port, uint16 value) {
	asm volatile("outw %%ax, %%dx"::"d" (port), "a" (value));
}

void outportl(uint16 port, uint32 value) {
	asm volatile("out %%eax, %%dx"::"d" (port), "a" (value));
}

