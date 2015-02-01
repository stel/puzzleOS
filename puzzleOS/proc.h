/*
 *  proc.h
 *  puzzleOS
 *
 *  Created by Dmitry on 25.04.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#ifndef _PROC_H_
#define _PROC_H_

#include "vmm.h"
#include "console.h"
#include "list.h"

#define MAX_PROC 256

#define PS_RUNABLE	0x00
#define PS_STOPED	0x01
#define PS_DEAD		0x02
#define PS_WFCHILD	0x03
#define PS_IDLE		0x04

#define P_KERNEL_STACK_SIZE 0x1000 // 4kb
#define P_USER_STACK_SIZE 0x1000 // 4kb

typedef struct context {
	int32 gs, fs, es, ds;     
    int32 edi, esi, ebp, esp, ebx, edx, ecx, eax;  
    int32 int_no, err_code;  
	int32 eip, cs, eflags;
} context_t;

typedef struct proc {
	int32 esp;
	
	int32 kernel_stack;
	int32 user_stack;
	
	page_directory_t *page_dir;
	
	uint32 pid;
	char name[32];
	uint32 *addr;
	int quantum;
	uint32 state;
	
	context_t *context;
	p_console *console;
	
	struct proc *parent;
} proc_t;

typedef struct proc_queue {
	proc_t *proc;
	struct proc_queue *prev;
	struct proc_queue *next;
} proc_queue_t;

typedef struct io_queue {
	proc_t *proc;
	struct io_queue *next;
} io_queue_t;

void proc_init();

proc_t *create_proc(uint32 eip);
proc_t *get_proc(uint32 pid);
void kill_proc(proc_t *proc);

void shedule();

extern proc_t *current_proc;

extern list_t *procs_list;
extern io_queue_t *io_queue;

#endif
