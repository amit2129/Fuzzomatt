#include <generic_ll.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void destroy_ll(LinkedList **head_ptr, void (*free_data)(void *data)) {
	LinkedList *cursor;
	LinkedList *cursor_next;
	if (!head_ptr)
		return;
	
	cursor = *head_ptr;
	if (!cursor)
		return;

	if (cursor->next)
		destroy_ll(&cursor->next, free_data);

	if (free_data) {free_data(cursor->data); }
	free(cursor);
}

int create_node(LinkedList **node_return, void *data) {
	*node_return = (LinkedList *) malloc(sizeof(LinkedList));
	if (!*node_return)
		return -ENOMEM;
	(*node_return)->data = data;
	(*node_return)->next = NULL;
}

int ll_insert_value(LinkedList **head_ptr, void *data) {
	LinkedList *cursor = *head_ptr;
	if (!cursor) {
		int ret = create_node(head_ptr, data);
		return ret;
	}
	while(cursor->next) { cursor = cursor->next; }
	return create_node(&(cursor->next), data);
}


int remove_data(LinkedList **head_ptr, void *data, int (*comparator)(LinkedList *a, LinkedList *b), void (*free_data)(void *data)) {
	LinkedList *cursor = *head_ptr;
	LinkedList tmpNode = {.data=data};
	remove_elem(head_ptr, &tmpNode, comparator, free_data);
}

int remove_elem(LinkedList **head_ptr, LinkedList *node, int (*comparator)(LinkedList *a, LinkedList *b), void (*free_data)(void *data)) {
	LinkedList *cursor = *head_ptr;
	LinkedList *prev = NULL;
	if (!cursor)
		return -ENODATA;

	if (comparator(cursor,node)) {
		if (free_data) {free_data(cursor->data);}
		*head_ptr = cursor->next;
		free(cursor);
		return 0;
	}
	prev = cursor;
	cursor = cursor->next;
	while (cursor) {
		if (comparator(cursor, node)) {
			if (free_data) {free_data(cursor->data);}
			prev->next = cursor->next;
			free(cursor);
			printf("57\n");
			return 0;
		}
		prev = cursor;
		cursor = cursor->next;
	}
	return -EINVAL;
}

void print_int(void *data) {
	printf("val: %d\n", *(int *)data);
}

int compare_int(LinkedList *a, LinkedList *b) {
	return *(int *)(a->data) == *(int *)(b->data);
}

void print_ll(LinkedList *head, void (*print_func)(void *data)) {
	while(head) {
		print_func(head->data);
		head = head->next;
	}
}

void test_ll() {
	printf("************** testing linked list *************\n");
	LinkedList *ll_v = NULL;
	LinkedList **ll = &ll_v;
	*ll = NULL;
	int a = 0;
	int b = 1;
	int c = 2;
	int d = 3;
	ll_insert_value(ll, &a);
	ll_insert_value(ll, &b);
	ll_insert_value(ll, &c);
	ll_insert_value(ll, &d);
	LinkedList *first = *ll;
	printf("return: %d\n", remove_data(ll, &a, compare_int, NULL));
	printf("return: %d\n", remove_data(ll, &a, compare_int, NULL));
	printf("return: %d\n", remove_data(ll, &c, compare_int, NULL));
	printf("return: %d\n", remove_data(ll, &d, compare_int, NULL));
	print_ll(*ll, print_int);

}

//int main() {
//	test_ll();
//	return 0;
//}
