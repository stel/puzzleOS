/*
 *  mmtest.c
 *  mmtest
 *
 *  Created by Dmitry on 13.06.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */
#include "puzzlestd.h"

int main(int argc, char **argv) {
	printf("Allocatind a (size = 4 bytes)..\n");
	int *a = malloc(4);
	printf("a: 0x%x\n", a);
	
	printf("Allocatind b (size = 4 bytes)..\n");
	int *b = malloc(4);
	printf("b: 0x%x\n", b);
	
	printf("Allocatind c (size = 4 bytes)..\n");
	int *c = malloc(4);
	printf("c: 0x%x\n", c);
	
	printf("Freeing c..\n");
	free(c);
	printf("Freeing b..\n");
	free(b);
	
	printf("Allocatind d (size = 12 bytes)..\n");
	int *d = malloc(12);
	printf("d: 0x%x\n", d);
	
	printf("Freeing d..\n");
	free(d);
	printf("Freeing a..\n");
	free(a);
	
	return 0;
}
