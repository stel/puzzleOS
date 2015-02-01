/*
 *  kill.c
 *  kill
 *
 *  Created by Dmitry on 15.06.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#include "puzzlestd.h"

int main(int argc, char **argv) {
	char str[256];
	int pid;
	printf("Target PID: ");
	gets(str);
	pid = atoi(str);
	kill(pid);
	printf("%d was killed.\n", pid);
	return 0;
}
