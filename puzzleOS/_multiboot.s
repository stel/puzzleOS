/*
 * multiboot.s
 * puzzleOS
 *
 * Created by Dmitry on 26.02.09.
 * Copyright 2009 Dmitry Obukhov. All rights reserved.
 */
 
 
.global multiboot_entry_point

.set MULTIBOOT_PAGE_ALIGN, 1<<0    
.set MULTIBOOT_MEM_INFO, 1<<1

.set MULTIBOOT_HEADER_MAGIC, 0x1BADB002
.set MULTIBOOT_HEADER_FLAGS, MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEM_INFO
.set MULTIBOOT_HEADER_CHECKSUM, -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)


	jmp		multiboot_entry_point
	
multiboot_header:
.align 4
.long MULTIBOOT_HEADER_MAGIC
.long MULTIBOOT_HEADER_FLAGS
.long MULTIBOOT_HEADER_CHECKSUM

.long  multiboot_header                     
.long  code                      
.long  bss                      
.long  kernel_end                      
.long  multiboot_entry_point

multiboot_entry_point:
	//mov		$(stack + STACKSIZE), %esp	// set up the stack
	//push	%eax						// Multiboot magic number
	//push	%ebx						// Multiboot data structure

	call	puzzle_init
	
	hlt
   
//.set STACKSIZE, 0x4000
//.comm stack, STACKSIZE, 32
   