#ifndef _GENERIC_LL
#define _GENERIC_LL

typedef struct _linked_list {
		void *data;
		struct _linked_list *next;
} LinkedList;

void destroy_ll(LinkedList **head_ptr, void (*free_data)(void *data));

int create_node(LinkedList **node_return, void *data);

int ll_insert_value(LinkedList **head_ptr, void *data);

int remove_data(LinkedList **head_ptr, void *data, int (*comparator)(LinkedList *a, LinkedList *b), void (*free_data)(void *data));

int remove_elem(LinkedList **head_ptr, LinkedList *node, int (*comparator)(LinkedList *a, LinkedList *b), void (*free_data)(void *data));

#endif
