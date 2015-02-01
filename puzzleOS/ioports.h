/*
 *  ioports.h
 *  puzzleOS
 *
 *  Created by Dmitry on 08.04.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#ifndef _IOPORTS_H_
#define _IOPORTS_H_

uint8 inportb(uint16 port);
uint16 inportw(uint16 port);
uint32 inportl(uint16 port);
void outportb(uint16 port, uint8 value);
void outportw(uint16 port, uint16 value);
void outportl(uint16 port, uint32 value);

#endif
