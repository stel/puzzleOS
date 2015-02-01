/*
 *  rd.c
 *  puzzleOS
 *
 *  Created by Dmitry on 02.06.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#include "puzzle.h"
#include "fs.h"
#include "rd.h"

rd_header_t *rd_header;     // The header.
rd_file_header_t *file_headers; // The list of file headers.
fs_node_t *rd_root;             // Our root directory node.
fs_node_t *rd_dev;              // We also add a directory node for /dev, so we can mount devfs later on.
fs_node_t *root_nodes;              // List of file nodes.
int nroot_nodes;                    // Number of file nodes.

dir_entry_t dir_entry;

static uint32 rd_read(fs_node_t *node, uint32 offset, uint32 size, uint8 *buffer) {
	rd_file_header_t header = file_headers[node->inode];
	
	if (offset > header.length)
		return 0;
	
	if (offset+size > header.length)
		size = header.length-offset;
	
	memcpy(buffer, (uint8 *) (header.offset+offset), size);
	
	return size;
}

static dir_entry_t *rd_readdir(fs_node_t *node, uint32 index) {
	if (node == rd_root && index == 0)
	{
		strcpy(dir_entry.name, "dev");
		dir_entry.name[3] = 0;
		dir_entry.inode_no = 0;
		return &dir_entry;
	}
	
	if (index-1 >= nroot_nodes)
		return 0;
	
	strcpy(dir_entry.name, root_nodes[index-1].name);
	
	dir_entry.name[strlen(root_nodes[index-1].name)] = 0; // Make sure the string is NULL-terminated.
	dir_entry.inode_no = root_nodes[index-1].inode;
	
	return &dir_entry;
}

static fs_node_t *rd_finddir(fs_node_t *node, char *name)
{
	if (node == rd_root &&
		!strcmp(name, "dev") )
		return rd_dev;
	
	int i;
	for (i = 0; i < nroot_nodes; i++)
		if (!strcmp(name, root_nodes[i].name))
		{
			return &root_nodes[i];
		}
			
	return 0;
}

fs_node_t *rd_init(uint32 location) {
	kprintf("Initializing RAM disk...\n");
	rd_header = (rd_header_t *)location;
	file_headers = (rd_file_header_t *) (location + sizeof(rd_header_t));
	
	// Initialise the root directory.
	rd_root = (fs_node_t*)kmalloc(sizeof(fs_node_t));
	strcpy(rd_root->name, "initrd");
	rd_root->inode = rd_root->length = 0;
	rd_root->type = FS_DIRECTORY;
	rd_root->read = 0;
	rd_root->write = 0;
	rd_root->open = 0;
	rd_root->close = 0;
	rd_root->readdir = &rd_readdir;
	rd_root->finddir = &rd_finddir;
	rd_root->ptr = NULL;
	
	// Initialise the /dev directory
	rd_dev = (fs_node_t*)kmalloc(sizeof(fs_node_t));
	strcpy(rd_dev->name, "dev");
	rd_dev->inode = rd_dev->length = 0;
	rd_dev->type = FS_DIRECTORY;
	rd_dev->read = 0;
	rd_dev->write = 0;
	rd_dev->open = 0;
	rd_dev->close = 0;
	rd_dev->readdir = &rd_readdir;
	rd_dev->finddir = &rd_finddir;
	rd_dev->ptr = NULL;
	
	root_nodes = (fs_node_t *)kmalloc(sizeof(fs_node_t) * rd_header->nfiles);
	nroot_nodes = rd_header->nfiles;
	
	int i;
	for (i = 0; i < rd_header->nfiles; i++)
	{
		// Edit the file's header - currently it holds the file offset
		// relative to the start of the ramdisk. We want it relative to the start
		// of memory.
		file_headers[i].offset += location;
		// Create a new file node.
		strcpy(root_nodes[i].name, file_headers[i].name);
		root_nodes[i].length = file_headers[i].length;
		root_nodes[i].inode = i;
		root_nodes[i].type = FS_FILE;
		root_nodes[i].read = &rd_read;
		root_nodes[i].write = 0;
		root_nodes[i].readdir = 0;
		root_nodes[i].finddir = 0;
		root_nodes[i].open = 0;
		root_nodes[i].close = 0;
	}
	
	kprintf("RAM disk at 0x%x\n", location);
	return rd_root;
}
