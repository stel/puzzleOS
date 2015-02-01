/*
 *  memory.h
 *  puzzleOS
 *
 *  Created by Dmitry on 05.04.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */
#ifndef _STRING_H_
#define _STRING_H_

#ifndef NULL
#define NULL ((void *) 0)
#endif

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned int size_t;
#endif

char *strcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);
int strcmp (const char *str1, const char *str2);
size_t strlen(const char *str);


void *memcpy(void *dest, const void *src, size_t count);
void *memset(void *dest, char val, size_t count);
unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count);


#endif
