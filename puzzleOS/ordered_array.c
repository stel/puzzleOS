/*
 *  ordered_array.c
 *  puzzleOS
 *
 *  Created by Dmitry on 10.05.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#include "puzzle.h"
#include "ordered_array.h"

uint8 default_predicate(ordered_array_item_t a, ordered_array_item_t b) {
	return (a < b) ? 1 : 0;
}

ordered_array_t create_ordered_array(size_t max_size, predicate_t predicate) {
	ordered_array_t ordered_array;
	
	ordered_array.items = (ordered_array_item_t *)kmalloc(max_size * sizeof(ordered_array_item_t));
	ordered_array.size = 0;
	ordered_array.max_size = max_size;
	ordered_array.predicate = predicate;
	
	return ordered_array;
}

ordered_array_t place_ordered_array(void *addr, size_t max_size, predicate_t predicate) {
	ordered_array_t ordered_array;
	
	ordered_array.items = (ordered_array_item_t *)addr;
	ordered_array.size = 0;
	ordered_array.max_size = max_size;
	ordered_array.predicate = predicate;
	
	return ordered_array;
}

void destroy_ordered_array(ordered_array_t *array) {
	//kfree(array->items);
}

uint32 insert_ordered_array(ordered_array_item_t item, ordered_array_t *array) {
	int i = 0;
	
	while (i < array->size && array->predicate(array->items[i], item))
		i++;
	
	if (i == array->size) 
		array->items[array->size++] = item;
	else
	{
		int j = i;
		ordered_array_item_t tmp = array->items[i];
		array->items[i] = item;
		
		while (j < array->size)
		{
			j++;
			ordered_array_item_t tmp2 = array->items[j];
			array->items[j] = tmp;
			tmp = tmp2;
		}
		array->size++;
	}
	
	return i;
}

ordered_array_item_t lookup_ordered_array(uint32 i, ordered_array_t *array) {
	return array->items[i];
}

void remove_ordered_array(uint8 i, ordered_array_t *array) {
	while (i < array->size)
	{
		array->items[i] = array->items[i+1];
		i++;
	}
	array->size--;
}
