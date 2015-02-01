/*
 *  memory.c
 *  puzzleOS
 *
 *  Created by Dmitry on 05.04.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#include "string.h"

char *strcpy(char *dest, const char *src) {
	do {
		*dest++ = *src;
	} while (*src++ != '\0');
	return dest;
}

char *strcat(char *dest, const char *src) {
	
	// STRCAT HERE!
	
	return dest;
}

int strcmp (const char *str1, const char *str2) {
	int i = 0;
	int res = 0;
	while(str1[i] != '\0' && str2[i] != '\0')
	{
		if(str1[i] != str2[i])
		{
			res = 1;
			break;
		}
		i++;
	}
	// why did the loop exit?
	if( (str1[i] == '\0' && str2[i] != '\0') || (str1[i] != '\0' && str2[i] == '\0') )
		res = 1;
	
	return res;
}

size_t strlen(const char *str)
{
    size_t len;
    for (len = 0; *str != '\0'; str++) 
		len++;
    return len;
}

void *memcpy(void *dest, const void *src, size_t count)
{
    const char *sp = (const char *)src;
    char *dp = (char *)dest;
    for (; count != 0; count--) 
		*dp++ = *sp++;
    return dest;
}

void *memset(void *dest, char val, size_t count)
{
    char *temp = (char *)dest;
    for ( ; count != 0; count--) 
		*temp++ = val;
    return dest;
}

unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count)
{
    unsigned short *temp = (unsigned short *)dest;
    for ( ; count != 0; count--) 
		*temp++ = val;
    return dest;
}
