/*
 *  puzzlestd.h
 *  puzzleOS
 *
 *  Created by Dmitry on 01.06.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#ifndef _PUZZLE_STD_H_
#define _PUZZLE_STD_H_

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned int size_t;
#endif

void exit(int errcode);

void putch(char c);
void puts(char *s);
int printf(const char *fmt, ...);
const char* strchr(const char *s, int c);
int atoi(const char *s);

char getch();
void gets(char *str);

int exec(char *s, int wait);

void waitpid(int pid);

void *malloc(size_t size);
void free(void *addr);

void kill(int pid);
void ps();

#endif
