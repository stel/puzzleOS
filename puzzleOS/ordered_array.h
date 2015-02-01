/*
 *  ordered_array.h
 *  puzzleOS
 *
 *  Created by Dmitry on 10.05.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */
#ifndef _ORDERED_ARRAY_H_
#define _ORDERED_ARRAY_H_

typedef void * ordered_array_item_t;

typedef uint8 (*predicate_t)(ordered_array_item_t a, ordered_array_item_t b);

typedef struct ordered_array {
	ordered_array_item_t *items;
	size_t size;
	size_t max_size;
	predicate_t predicate;
} ordered_array_t;

uint8 default_predicate(ordered_array_item_t a, ordered_array_item_t b);

ordered_array_t create_ordered_array(size_t max_size, predicate_t predicate);
ordered_array_t place_ordered_array(void *addr, size_t max_size, 
									predicate_t predicate);

void destroy_ordered_array(ordered_array_t *array);

uint32 insert_ordered_array(ordered_array_item_t item, ordered_array_t *array);
ordered_array_item_t lookup_ordered_array(uint32 i, ordered_array_t *array);
void remove_ordered_array(uint8 i, ordered_array_t *array);

#endif
