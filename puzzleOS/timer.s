/*
 * timer.s
 * puzzleOS
 *
 * Created by Dmitry on 16.05.09.
 * Copyright 2009 Dmitry Obukhov. All rights reserved.
 */
 
//.global timer_irq
//
//.extern timer_int
//.extern pic_release
//
//timer_irq:
//	push	$0x00
//	call	pic_release
//	add		$0x04, %esp
//	call	timer_int