/*
 *  fs.c
 *  puzzleOS
 *
 *  Created by Dmitry on 11.05.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#include "puzzle.h"
#include "fs.h"

#define FS_NODE_TYPE(fs_node) fs_node->type & 0x07

fs_node_t *fs_root;

uint32 fs_open(fs_node_t *fs_node) {
	if (fs_node->open)
		return fs_node->open(fs_node);
	else
		return 0;
}

uint32 fs_close(fs_node_t *fs_node) {
	if (fs_node->close)
		return fs_node->close(fs_node);
	else
		return 0;
}

uint32 fs_read(fs_node_t *fs_node, uint32 offset, size_t size, uint8 *buffer) {
	if (fs_node->read)
		return fs_node->read(fs_node, offset, size, buffer);
	else
		return 0;
}

uint32 fs_write(fs_node_t *fs_node, uint32 offset, size_t size, uint8 *buffer) {
	if (fs_node->write)
		return fs_node->write(fs_node, offset, size, buffer);
	else
		return 0;
}

dir_entry_t *fs_readdir(fs_node_t *fs_node, uint32 index) {
	if ((fs_node->type == FS_DIRECTORY) && fs_node->readdir)
		return fs_node->readdir(fs_node, index);
	else
		return 0;
}

fs_node_t *fs_finddir(fs_node_t *fs_node, char *name) {
	if ((FS_NODE_TYPE(fs_node) & FS_DIRECTORY) && fs_node->finddir)
		return fs_node->finddir(fs_node, name);
	else
		return 0;
}
