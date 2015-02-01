/*
 *  syscalls.h
 *  puzzleOS
 *
 *  Created by Dmitry on 17.05.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

typedef int (*fn_ptr)();

void syscalls_init();

extern int sys_exit();
extern int sys_putch();
extern int sys_getch();
extern int sys_getpid();
extern int sys_exec();
extern int sys_waitpid();
extern int sys_malloc();
extern int sys_free();
extern int sys_kill();
extern int sys_ps();

fn_ptr syscalls_table[] = {
	sys_exit, sys_putch, sys_getch, sys_getpid, sys_exec, sys_waitpid, 
	sys_malloc, sys_free, sys_kill, sys_ps
};

#endif
