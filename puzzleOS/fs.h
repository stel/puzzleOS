/*
 *  fs.h
 *  puzzleOS
 *
 *  Created by Dmitry on 11.05.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#ifndef _FS_H_
#define _FS_H_

#define FS_FILE        0x01
#define FS_DIRECTORY   0x02
#define FS_CHARDEVICE  0x03
#define FS_BLOCKDEVICE 0x04
#define FS_PIPE        0x05
#define FS_SYMLINK     0x06
#define FS_MOUNTPOINT  0x08

struct fs_node;

typedef struct dir_entry {
	char name[128];
	uint32 inode_no;     // Inode number. Required by POSIX.
} dir_entry_t;

typedef uint32 (*fs_open_t)(struct fs_node *);
typedef uint32 (*fs_close_t)(struct fs_node *);
typedef uint32 (*fs_read_t)(struct fs_node *, uint32 offset, size_t size, uint8 *buffer);
typedef uint32 (*fs_write_t)(struct fs_node *, uint32 ofset, size_t size, uint8 *buffer);

typedef struct dir_entry *(*fs_readdir_t)(struct fs_node *, uint32 index);
typedef struct fs_node *(*fs_finddir_t)(struct fs_node *, char *name);

typedef struct fs_node {
	char name[128];
	uint32 type;
	uint32 inode;
	uint32 length;
	
	fs_open_t open;
	fs_close_t close;
	fs_read_t read;
	fs_write_t write;
	
	fs_readdir_t readdir;
	fs_finddir_t finddir;
	
	struct fs_node *ptr;
} fs_node_t;

uint32 fs_open(fs_node_t *fs_node);
uint32 fs_close(fs_node_t *fs_node);
uint32 fs_read(fs_node_t *fs_node, uint32 offset, size_t size, uint8 *buffer);
uint32 fs_write(fs_node_t *fs_node, uint32 offset, size_t size, uint8 *buffer);
dir_entry_t *fs_readdir(fs_node_t *fs_node, uint32 index);
fs_node_t *fs_finddir(fs_node_t *fs_node, char *name);

extern fs_node_t *fs_root;

#endif
