/*
 *  psyslib.h
 *  puzzleOS
 *
 *  Created by Dmitry on 18.04.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#ifndef _PSYSLIB_H_
#define _PSYSLIB_H_

int kprintf(const char *fmt, ...);

proc_t *exec(char *file_name);

#endif
