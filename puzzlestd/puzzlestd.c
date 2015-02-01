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

void exit(int errcode) {
	asm volatile("int $0x30"::"a" (0), "b" (errcode));
}

void waitpid(int pid) {
	asm volatile("int $0x30"::"a" (5), "b" (pid));
}

int exec(char *s, int wait) {
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
static char buf[256];
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

void *malloc(size_t size) {
	void *p;
	asm volatile("int $0x30":"=a" (p):"a" (6), "b" (size));
	return p;
}

void free(void *addr) {
	asm volatile("int $0x30"::"a" (7), "b" (addr));
}

void kill(int pid) {
	asm volatile("int $0x30"::"a" (8), "b" (pid));
}

void ps() {
	asm volatile("int $0x30"::"a" (9));
}

const char* strchr(const char *s, int c) {
	do {
		if ((unsigned)*s == (unsigned)c)
			return s;
	} while (*(++s) != 0);
	
	return 0;
}

int atoi(const char *s) {
	static const char digits[] = "0123456789";  /* legal digits in order */
	unsigned val=0;         /* value we're accumulating */
	int neg=0;              /* set to true if we see a minus sign */
	
	/* skip whitespace */
	while (*s==' ' || *s=='\t') {
		s++;
	}
	
	/* check for sign */
	if (*s=='-') {
		neg=1;
		s++;
	} else if (*s=='+') {
		s++;
	}
	
	/* process each digit */
	while (*s) {
		const char *where;
		unsigned digit;
		
		/* look for the digit in the list of digits */
		where = strchr(digits, *s);
		if (where==0) {
			/* not found; not a digit, so stop */
			break;
		}
		
		/* get the index into the digit list, which is the value */
		digit = (where - digits);
		
		/* could (should?) check for overflow here */
		
		/* shift the number over and add in the new digit */
		val = val*10 + digit;
		
		/* look at the next character */
		s++;
	}
	
	if (neg) {
		return -val;
	}
	
	return val;
}
