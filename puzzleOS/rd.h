/*
 *  rd.h
 *  puzzleOS
 *
 *  Created by Dmitry on 02.06.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#ifndef _RD_H_
#define _RD_H_

typedef struct rd_header {
	uint32 nfiles;
} rd_header_t;

typedef struct rd_file_header {
	uint8 magic;
	char name[64];
	uint32 offset;
	uint32 length;
} rd_file_header_t;

fs_node_t *rd_init(uint32 location);

#endif
