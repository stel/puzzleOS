/*
 *  panic.c
 *  puzzleOS
 *
 *  Created by Dmitry on 07.05.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#include "puzzle.h"

void panic(const char *msg, const char *file, uint32 line) {
	kprintf("KERNEL PANIC: %s (%s:%d)", msg, file, line);
	for(;;);
}
