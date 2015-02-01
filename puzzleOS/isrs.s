/*
 *  isrs.s
 *  puzzleOS
 *
 *  Created by Dmitry on 12.04.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

.global isr00
.global isr01
.global isr02
.global isr03
.global isr04
.global isr05
.global isr06
.global isr07
.global isr08
.global isr09
.global isr0A
.global isr0B
.global isr0C
.global isr0D
.global isr0E
.global isr0F
.global isr10
.global isr11
.global isr12_1F

// Syscalls
.global isr30

.extern isr

isr00:
	push	$0x00
	push	$0x00
	jmp		isr_wrapper
	
isr01:
	push	$0x00
	push	$0x01
	jmp		isr_wrapper
	
isr02:
	push	$0x00
	push	$0x02
	jmp		isr_wrapper
	
isr03:
	push	$0x00
	push	$0x03
	jmp		isr_wrapper
	
isr04:
	push	$0x00
	push	$0x04
	jmp		isr_wrapper
	
isr05:
	push	$0x00
	push	$0x05
	jmp		isr_wrapper
	
isr06:
	push	$0x00
	push	$0x06
	jmp		isr_wrapper
	
isr07:
	push	$0x00
	push	$0x07
	jmp		isr_wrapper
	
isr08:
	push	$0x00
	push	$0x08
	jmp		isr_wrapper
	
isr09:
	push	$0x00
	push	$0x09
	jmp		isr_wrapper
	
isr0A:
	push	$0x00
	push	$0x0A
	jmp		isr_wrapper
	
isr0B:
	push	$0x00
	push	$0x0B
	jmp		isr_wrapper
	
isr0C:
	push	$0x00
	push	$0x0C
	jmp		isr_wrapper
	
isr0D:
	push	$0x00
	push	$0x0D
	jmp		isr_wrapper

// Page Fault
isr0E:
	//pushl	$0x03
	pushl	$0x0E
	jmp		isr_wrapper
	
isr0F:
	push	$0x00
	push	$0x0F
	jmp		isr_wrapper
	
isr10:
	push	$0x00
	push	$0x10
	jmp		isr_wrapper
	
isr11:
	push	$0x00
	push	$0x11
	jmp		isr_wrapper
	
isr12_1F:
	push	$0x00
	push	$0x12
	jmp		isr_wrapper

// Syscalls
isr30:
	push	$0x00
	push	$0x30
	jmp		isr_syscall
	
isr_wrapper:
	pusha
	push	%ds
	push	%es
	push	%fs
    push	%gs
    movw	$0x10, %ax
    mov		%ax, %ds
    mov		%ax, %es
    mov		%ax, %fs
    mov		%ax, %gs
	
	mov		%esp, %eax	// interrup_info
    
	pushl	%esp
	call	isr
	
	popl	%eax
	
    pop		%gs
    pop		%fs
    pop		%es
    pop		%ds
    popa
    add		$0x08, %esp
	// sti - iret восстановит старый EFLAGS
    iret         
	
isr_syscall:
	pusha
	pushl	%ds
	pushl	%es
	pushl	%fs
    pushl	%gs
    
	movl	$0x10, %eax
    mov		%ax, %ds
    mov		%ax, %es
    mov		%ax, %fs
    mov		%ax, %gs
	
	mov		%esp, %eax
	
	movl	$0xFFFF, %esp	// set kernel stack
	
	pushl	%eax		// interrupt info / old task kernel stack

	call	syscalls_interrupt_handler
	
	//call	shedule
	test	%eax, %eax
	jz		get_esp
	
	call	switch_task
	add		$0x04, %esp	// remove esp pointer (interrupt info / old task kernel stack)
	jmp		set_esp

get_esp:
	pop		%eax
	
set_esp:	
	movl	%eax, %esp	// new task kernel stack
	
    popl	%gs
    popl	%fs
    popl	%es
    popl	%ds
    popa
	add		$0x08, %esp	// delete int_no, err_code
   	iret
