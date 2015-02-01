/*
 *  psh.c
 *  psh
 *
 *  Created by Dmitry on 13.06.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#include "puzzlestd.h"
#include "string.h"
#include "psh.h"

void prompt() {
	printf("> ");
}

void cmd_ps() {
	ps();
}

int main(int argc, char **argv) {
	char str[256];
	printf("Welcome to psh v%s\n", VERSION);
	while (1) {
		prompt();
		gets(str);
		if (!strlen(str))
			continue;
		//ps();
		
		if (!exec(str, 1))
			printf("%s: command not found!\n", str);
	}
	return 0;
}
