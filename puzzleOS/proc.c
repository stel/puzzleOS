/*
 *  proc.c
 *  puzzleOS
 *
 *  Created by Dmitry on 25.04.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#include "puzzle.h"
#include "vmm.h"
#include "console.h"
#include "proc.h"

list_t *procs_list;

proc_t *current_proc;
static proc_t *idle_proc;

io_queue_t *io_queue = NULL;

static void idle() {
	for(;;);
}

void proc_init() {
	kprintf("Initializing process manager...\n");
	procs_list = kmalloc(sizeof(list_t));
	idle_proc = create_proc((uint32)idle);
	strcpy(idle_proc->name, "sysidle");
	idle_proc->state = PS_IDLE;
	//current_proc = idle_proc;
	shedule();
}

proc_t *create_proc(uint32 eip) {
	proc_t *p;
	uint32 *kstacksetup;
	
	p = kmalloc(sizeof(proc_t));
	p->pid = procs_list->items_count;
	
	p->user_stack = (uint32)kmalloc(P_USER_STACK_SIZE) + P_USER_STACK_SIZE / 4;
	
	kstacksetup = kmalloc(P_KERNEL_STACK_SIZE) + P_KERNEL_STACK_SIZE / 4;
	
	*--kstacksetup = 0x23;		// Ring3 ss
	*--kstacksetup = p->user_stack;	// Ring3 esp
	*--kstacksetup = 0x0202;	// eflags
	*--kstacksetup = 0x1B;		// cs
	*--kstacksetup = eip;		// eip
	
	*--kstacksetup = 0;		// eip
	*--kstacksetup = 0;		// eip
	
	*--kstacksetup = 0;			// ebp
	*--kstacksetup = 0;			// esp
	*--kstacksetup = 0;			// edi
	*--kstacksetup = 0;			// esi
	*--kstacksetup = 0;			// edx
	*--kstacksetup = 0;			// ecx
	*--kstacksetup = 0;			// ebx
	*--kstacksetup = 0;			// eax
	*--kstacksetup = 0x23;		// ds
	*--kstacksetup = 0x23;		// es
	*--kstacksetup = 0x23;		// fs
	*--kstacksetup = 0x23;		// gs
	
	p->esp = p->kernel_stack = (uint32)kstacksetup;
	
	p->page_dir = vmm_create_page_directory();
	vmm_copy_page_directory(p->page_dir, kernel_page_dir);
	
	p->context = (context_t *)kstacksetup;
	p->console = current_console;		
	
	p->state = PS_RUNABLE;
	list_push_back(procs_list, p);
	
	p->parent = current_proc;
	
	return p;
}

proc_t *get_proc(uint32 pid) {
	proc_t *tmp;
	int i;
	
	for (i = 0; i < procs_list->items_count; i++) {
		tmp = list_get_value(procs_list, i);
		if (tmp->pid == pid)
			return tmp;
	}
	
	return 0;
}

void kill_proc(proc_t *proc) {
	
	if (!proc->pid)
		return;
	
	if (!proc->parent->pid)
		return;
	
	if (proc->parent->state == PS_WFCHILD)
		proc->parent->state = PS_RUNABLE;
	
	list_delete(procs_list, proc);
	
	kfree((void *)proc->user_stack);
	kfree((void *)proc->kernel_stack);
	//kfree(proc->context); - потому что == кернел стек
	if (proc == current_proc) {
		current_proc = 0;
		vmm_set_current_page_directory(kernel_page_dir);
	}
	kfree(proc->page_dir);
	
	kfree(proc);
}

void shedule() {
	proc_t *tmp;
	
	int i = 0;
	do {
		tmp = list_pop_front(procs_list);
		list_push_back(procs_list, tmp);
		i++;
	} while (tmp->state != PS_RUNABLE && i < procs_list->items_count);
	
	if (tmp->state != PS_RUNABLE)
		current_proc = idle_proc;
	else
		current_proc = tmp;
}

uint32 switch_task(uint32 old_esp) {
	if (current_proc != 0) {
		current_proc->esp = old_esp;
		current_proc->context = (context_t *)old_esp;
	}
	
	shedule();
	
	tss->esp0 = current_proc->kernel_stack;
	
	vmm_set_current_page_directory(current_proc->page_dir);
	
	return current_proc->esp;
}
