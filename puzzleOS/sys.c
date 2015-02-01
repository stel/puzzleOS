/*
 *  sys.c
 *  puzzleOS
 *
 *  Created by Dmitry on 12.06.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */
#include "puzzle.h"
#include "proc.h"
#include "console.h"


extern io_queue_t *io_queue;
extern void kputch(char c);
extern int reshedule;

void sys_exit() {
	kill_proc(current_proc);
	reshedule = 1;
}

void sys_putch(char c) {
	console_write(current_proc->console, c);
}

char sys_getch() {
	//char c = console_read(current_proc->console);
//	
//	if (c)
//		return c;
	
	current_proc->state = PS_STOPED;
	
	list_push_back(current_proc->console->io_queue, current_proc);
	
	reshedule = 1;
	return 0;
}

int sys_getpid() {
	return current_proc->pid;
}

int sys_exec(char *file_name, int wait) {
	proc_t *proc = exec(file_name);
	if (proc) {
		if (wait) {
			current_proc->state = PS_WFCHILD;
			reshedule = 1;
		}
		
		return proc->pid;
	}
		
	return 0;
}

int sys_waitpid(int pid) {
	current_proc->state = PS_WFCHILD;
	reshedule = 1;
	return 0;
}

void *sys_malloc(size_t size) {
	return kmalloc(size);
}

void sys_free(void *addr) {
	kfree(addr);
}

void sys_kill(uint32 pid) {
	proc_t *proc = get_proc(pid);
	if (!proc)
		return;
	kill_proc(proc);
}

static const char *proc_state[] = {
"RUNNING",
"STOPPED",
"DEAD",
"WFCHILD",
"IDLE"
};

void sys_ps() {
	proc_t *proc;
	int i;
	kprintf("#\tNAME\t  PID\tPPID\tSTATE\t  CON\n");
	kprintf("----------------------------------------------\n");
	for (i = 0; i < procs_list->items_count; i++) {
		proc = list_get_value(procs_list, i);
		kprintf("%d\t%s\t  %d\t%d\t%s\t  con%d\n", i + 1,
				proc->name, 
				proc->pid,
				(proc->parent) ? proc->parent->pid : -1,
				proc_state[proc->state], 
				proc->console->num);
	}
}
