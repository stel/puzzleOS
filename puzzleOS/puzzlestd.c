/*
 *  puzzlestd.c
 *  puzzleOS
 *
 *  Created by Dmitry on 01.06.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#include "stdarg.h"
#include "puzzlestd.h"

void waitpid(int pid) {
	asm volatile("int $0x30"::"a" (5), "b" (pid));
}

int execv(char *s, int wait) {
	int pid;
	asm volatile("int $0x30":"=a" (pid):"a" (4), "b" (s), "c" (wait));
	return pid;
}

void putch(char c) {
	asm volatile("int $0x30"::"a" (1), "b" (c));
}

void puts(char *s) {
	while (*s)
		putch(*s++);
}

int vsprintf(char *buf, const char *fmt, va_list args);
static char buf[1024];
int printf(const char *fmt, ...) {
	int i;
	va_list args;
	
	va_start(args, fmt);
	i=vsprintf(buf,fmt,args);
	va_end(args);
	
	puts(buf);
	return i;
}


char getch() {
	char c;
	asm volatile("int $0x30":"=a" (c):"a" (2));
	return c;
}

void gets(char *str) {
	char b=getch();
	while (b != '\n') {
			if (b == 0x08)
				str--;
			else if (b > 0)
				*str++ = b;
		b = getch();
	}
	*str = '\0';
}

