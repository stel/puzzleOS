/*
 * startup.s
 * puzzleOS
 *
 * Created by Dmitry on 05.04.09.
 * Copyright 2009 Dmitry Obukhov. All rights reserved.
 */
 
.global puzzle_init

.extern kmain
.extern current_proc

puzzle_init:
	movl	$0xFFFF, %esp
	push    %ebx

	call	arch_init
	
	jmp		$0x08, $update_regs
update_regs:
	movl	$0x10, %eax
	mov		%ax, %ds
	mov		%ax, %gs
	mov		%ax, %fs
	mov		%ax, %es
	mov		%ax, %ss
	
	call	interrupts_init
	call	console_init
	call	syscalls_init
	
	call	timer_init
	
	call	kmain
	
	movl	current_proc, %eax
	
	movl	(%eax), %esp	// current_proc->esp
	
    popl	%gs
    popl	%fs
    popl	%es
    popl	%ds
	popa
	add		$8, %esp	// int err code, int no
   	iret
	
	//sti
