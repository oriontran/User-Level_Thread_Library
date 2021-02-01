#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "queue.h"

struct node {
	void *item;
	struct node* next;
};
typedef struct node node;

struct queue {
	struct node *head, *tail;
	int curr_length;
};

queue_t queue_create(void)
{
	queue_t queue = (queue_t) malloc(sizeof(struct queue));
	if (!queue)
		return NULL;
	queue -> head = NULL;
	queue -> tail = NULL;
	queue -> curr_length = 0;
	return queue;
}

int queue_destroy(queue_t queue)
{
	if (!queue || queue -> head != NULL)
		return -1;
	free(queue);
	return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
	/* declare new node and initialize it */
	node *new_node;
	new_node = malloc(sizeof(node));
	if (!new_node || !queue || !data)
		return -1;
	new_node -> next = NULL;
	new_node -> item = data;

	if (queue -> head == NULL) { 
		queue -> head = new_node;
		queue -> tail = new_node;
	} else {
		queue -> tail -> next = new_node;
		queue -> tail = new_node;
	}

	queue -> curr_length++;
	return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
	node *temp = queue -> head;
	if (!queue || !data || temp == NULL)
		return -1;
	*data = queue -> head -> item;
	queue -> head = queue -> head -> next;
	if (queue -> head == NULL)
		queue -> tail = NULL;
	queue -> curr_length--;
	free(temp);
	temp = NULL;
	return 0;
}

int queue_delete(queue_t queue, void *data)
{
	node *temp_prev = queue -> head;
	node *temp = queue -> head -> next;
	/* if the very first item is equal to data */
	if (queue -> head -> item == data) {
		queue -> head = queue -> head -> next;
		free(temp_prev);
		return 0;
	}
	
	/* all other items */
	while (temp) {
		if (temp -> item == data) {
			temp_prev -> next = temp -> next;
			free(temp);
			temp = NULL;
			return 0;
		}
		temp_prev = temp;
		temp = temp -> next;
	}
	queue -> curr_length--;
	return -1;
}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{

	if (!queue || !func)
		return -1;
	node *temp = queue -> head;
	while (temp) {
		if (func(temp -> item, arg) == 1) {
			if (data != NULL) 
				*data = temp -> item;
			break;
		}
		temp = temp -> next;
	}
	return 0;
}

int queue_length(queue_t queue)
{
	return queue -> curr_length;
}
