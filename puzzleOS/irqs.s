/*
 * irqs.s
 * puzzleOS
 *
 * Created by Dmitry on 19.04.09.
 * Copyright 2009 Dmitry Obukhov. All rights reserved.
 */

.global irq00
.global irq01
.global irq02
.global irq03
.global irq04
.global irq05
.global irq06
.global irq07
.global irq08
.global irq09
.global irq0A
.global irq0B
.global irq0C
.global irq0D
.global irq0E
.global irq0F

.extern pic_irq
.extern timer_interrupt_handler

irq00:
	push	$0x00
	push	$0x00
	jmp		timer_irq
	
irq01:
	push	$0x00
	push	$0x01
	jmp		irq_wrapper
	
irq02:
	push	$0x00
	push	$0x02
	jmp		irq_wrapper
	
irq03:
	push	$0x00
	push	$0x03
	jmp		irq_wrapper
	
irq04:
	push	$0x00
	push	$0x04
	jmp		irq_wrapper
	
irq05:
	push	$0x00
	push	$0x05
	jmp		irq_wrapper
	
irq06:
	push	$0x00
	push	$0x06
	jmp		irq_wrapper
	
irq07:
	push	$0x00
	push	$0x07
	jmp		irq_wrapper
	
irq08:
	push	$0x00
	push	$0x08
	jmp		irq_wrapper
	
irq09:
	push	$0x00
	push	$0x09
	jmp		irq_wrapper
	
irq0A:
	push	$0x00
	push	$0x0A
	jmp		irq_wrapper
	
irq0B:
	push	$0x00
	push	$0x0B
	jmp		irq_wrapper
	
irq0C:
	push	$0x00
	push	$0x0C
	jmp		irq_wrapper
	
irq0D:
	push	$0x00
	push	$0x0D
	jmp		irq_wrapper
	
irq0E:
	push	$0x00
	push	$0x0E
	jmp		irq_wrapper
	
irq0F:
	push	$0x00
	push	$0x0F
	jmp		irq_wrapper
	
irq_wrapper:
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
    pushl	%eax		// interrupt_info pointer

	call	pic_irq
	
	popl	%eax
	
    popl	%gs
    popl	%fs
    popl	%es
    popl	%ds
    popa
    add		$0x08, %esp
   	iret
	
timer_irq:
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

	call	pic_irq
	
	//call	shedule
	
	call	switch_task
	add		$0x04, %esp	// remove esp pointer (interrupt info / old task kernel stack)
	
	movl	%eax, %esp	// new task kernel stack
	
    popl	%gs
    popl	%fs
    popl	%es
    popl	%ds
    popa
	add		$0x08, %esp	// delete int_no, err_code
   	iret
