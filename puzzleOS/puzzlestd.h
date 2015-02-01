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

int exit(int errcode);

void putch(char c);
void puts(char *s);
int printf(const char *fmt, ...);

char getch();
void gets(char *str);

int execv(char *s, int wait);

void waitpid(int pid);

#endif
