/*
 *  list.h
 *  puzzleOS
 *
 *  Created by Dmitry on 13.06.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#ifndef _LIST_H_
#define _LIST_H_

typedef struct list_item {
	void *value;
	struct list_item *prev;
	struct list_item *next;
} list_item_t;

typedef struct list {
	list_item_t *first_item;
	list_item_t *last_item;
	int items_count;
} list_t;

void *list_pop_front(list_t *list);
void *list_push_back(list_t *list, void *value);
void *list_get_value(list_t *list, uint32 item_num);
void list_delete(list_t *list, void *value);

#endif
