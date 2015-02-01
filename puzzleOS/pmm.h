/*
 *  pmm.h
 *  puzzleOS
 *
 *  Created by Dmitry on 13.05.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#ifndef _PMM_H_
#define _PMM_H_

#define PMM_FRAMES_PER_INT 32 // 32 фрейма в 1 элементе карты памти

#define PMM_FRAME_SIZE 0x1000 // 4Kb

void pmm_init(uint32 mem_size);

uint32 pmm_get_free_frame();
void pmm_alloc_frame(uint32 frame);
void pmm_free_frame(uint32 frame);
uint32 pmm_test_frame(uint32 frame);

uint32 pmm_get_free_frame_addr();
void pmm_alloc_frame_addr(uint32 addr);
void pmm_free_frame_addr(uint32 addr);
uint32 pmm_test_frame_addr(uint32 addr);

#endif
