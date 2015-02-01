/*
 *  pmm.c
 *  puzzleOS
 *
 *  Created by Dmitry on 13.05.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#include "puzzle.h"
#include "pmm.h"

#define FRAME_TO_ADDR(frame) (frame * PMM_FRAME_SIZE)
#define ADDR_TO_FRAME(addr) (addr / PMM_FRAME_SIZE)

#define PMM_ALIGN_CHECK(addr) if (addr % PMM_FRAME_SIZE) \
								PANIC("Addres is not frame-aligned!");

static uint32 *pmm_memory_map;
static uint32 pmm_used_frames;
static uint32 pmm_max_frames;

void pmm_init(uint32 mem_size) {
	pmm_max_frames = mem_size / PMM_FRAME_SIZE;
	pmm_used_frames = 0;
	
	pmm_memory_map = kmalloc((pmm_max_frames / PMM_FRAMES_PER_INT) * sizeof(uint32));
	kprintf("%d frames allocated\n", pmm_max_frames);
	kprintf("Memory map at 0x%x\n", pmm_memory_map);
}

uint32 pmm_get_free_frame() {
	int i, j;
	for (i = 0; i < pmm_max_frames; i++) {
		if (pmm_memory_map[i] != 0xFFFFFFFF) {
			for (j = 0; j < PMM_FRAMES_PER_INT; j++) {
				if ((pmm_memory_map[i] & (0x01 << j)) == 0)
					return i * PMM_FRAMES_PER_INT + j;
			}	
		}
	}
	return (uint32)-1;
}

void pmm_alloc_frame(uint32 frame) {
	
	if (frame > pmm_max_frames - 1)
		PANIC("No such frame!");
	
	uint32 index = frame / PMM_FRAMES_PER_INT;
	uint32 offset = frame % PMM_FRAMES_PER_INT;
	
	pmm_memory_map[index] |= (0x01 << offset);
	
	pmm_used_frames++;
}

void pmm_free_frame(uint32 frame) {
	
	if (frame > pmm_max_frames - 1)
		PANIC("No such frame!");
	
	uint32 index = frame / PMM_FRAMES_PER_INT;
	uint32 offset = frame % PMM_FRAMES_PER_INT;
	
	pmm_memory_map[index] &= ~(0x01 << offset);
	
	pmm_used_frames--;
}

uint32 pmm_test_frame(uint32 frame) {
	
	if (frame > pmm_max_frames - 1)
		PANIC("No such frame!");
	
	uint32 index = frame / PMM_FRAMES_PER_INT;
	uint32 offset = frame % PMM_FRAMES_PER_INT;
	
	return (pmm_memory_map[index] & ((0x01 << offset) > 0)) ? 1 : 0;
}

uint32 pmm_get_free_frame_addr() {
	uint32 frame = pmm_get_free_frame();
	return (frame == -1) ? -1 : FRAME_TO_ADDR(frame);
}

void pmm_alloc_frame_addr(uint32 addr) {
	PMM_ALIGN_CHECK(addr);
	pmm_alloc_frame(ADDR_TO_FRAME(addr));
}

void pmm_free_frame_addr(uint32 addr) {
	PMM_ALIGN_CHECK(addr);
	pmm_free_frame(ADDR_TO_FRAME(addr));
}

uint32 pmm_test_frame_addr(uint32 addr) {
	PMM_ALIGN_CHECK(addr);
	return pmm_test_frame(ADDR_TO_FRAME(addr));
}
