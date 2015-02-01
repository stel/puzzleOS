/*
 *  list.c
 *  puzzleOS
 *
 *  Created by Dmitry on 13.06.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#include "puzzle.h"
#include "list.h"

void *list_pop_front(list_t *list) {
	if (!list->items_count)
		return 0;
	
	list_item_t *item = list->first_item;
	
	list->first_item = list->first_item->next;
	if (list->first_item != 0)
		list->first_item->prev = 0;
	else
		list->last_item = 0;
	
	list->items_count--;
	
	void *value = item->value;
	kfree(item);
	return value;
}

void *list_push_back(list_t *list, void *value) {
	
	list_item_t *new_item = kmalloc(sizeof(list_item_t));
	new_item->value = value;
	new_item->prev = list->last_item;
	new_item->next = 0;
	
	if (!list->items_count)
		list->first_item = new_item;
	else
		list->last_item->next = new_item;
	
	list->last_item = new_item;
	
	list->items_count++;
	return new_item->value;
}

void *list_get_value(list_t *list, uint32 item_num) {
	list_item_t *item = list->first_item;
	
	if (item_num >= list->items_count)
		return 0;
	
	int i;
	for (i = 0; i < item_num; i++)
		item = item->next;
	
	return item->value;
}

static list_item_t *list_get_item(list_t *list, void *value) {
	list_item_t *item = list->first_item;
	
	while (item) {
		if (item->value == value)
			return item;
		item = item->next;
	}
	return 0;
}

void list_delete(list_t *list, void *value) {
	list_item_t *item = list_get_item(list, value);
	
	if (item == 0)
		return;
	
	if (item->prev == 0)
		list->first_item = item->next;
	else
		item->prev->next = item->next;
	
	if (item->next == 0)
		list->last_item = item->prev;
	else
		item->next->prev = item->prev;
	
	list->items_count--;
	
	kfree(item);
}
