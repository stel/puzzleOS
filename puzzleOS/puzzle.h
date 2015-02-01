/*
 *  puzzle.h
 *  puzzleOS
 *
 *  Created by Dmitry on 18.04.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#include "types.h"
#include "string.h"
#include "proc.h"
#include "psyslib.h"

#include "i586.h"

#include "heap.h"

#define PUZZLE_RELEASE 0
#define PUZZLE_VERSION 1

#define PANIC(msg) panic(msg, __FILE__, __LINE__);

void panic(const char *msg, const char *file, uint32 line);
