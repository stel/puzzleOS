/*
 *  kprintf.c
 *  puzzleOS
 *
 *  Created by Dmitry on 18.04.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#include "stdarg.h"

int vsprintf(char *buf, const char *fmt, va_list args);

void kputch(char c);

static char buf[1024];

void kputs(char *s) {
	while (*s) {
		kputch(*s++);
	}
}

int kprintf(const char *fmt, ...) {
	int i;
	va_list args;
	
	va_start(args, fmt);
	i=vsprintf(buf,fmt,args);
	va_end(args);
	
	kputs(buf);
	return i;
}
