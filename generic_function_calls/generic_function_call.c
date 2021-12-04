#include <stdio.h>
#include <generic_ll.h>

int print_int_loc(int a, int b) {
	printf("int received: %d, %d\n", a, b);
}

int change_int(int *a) {
	printf("int received: %d\n", *a);
	*a+= 1;
}
#define P99_PROTECT(...) __VA_ARGS__

#define EXPOSE_CALLABLE(func_name, input_lines, in_args) \
		void _callable_##func_name(LinkedList *input_args) { \
			input_lines \
			func_name in_args; \
		}

#define INPUT_LINE(type, name) \
		type *name = (type *) input_args->data; \
		input_args = input_args->next;

EXPOSE_CALLABLE(print_int_loc, INPUT_LINE(int, arg1) INPUT_LINE(int, arg2), (*arg1,*arg2));
EXPOSE_CALLABLE(change_int, INPUT_LINE(int*, arg1), (*arg1));

int compare_int(LinkedList *a, LinkedList *b);
int main() {
	
	printf("************** testing linked list *************\n");
	LinkedList **ll =(LinkedList **) malloc(sizeof(LinkedList *));
	*ll = NULL;
	int a = 8;
	int b = 546213;
	int *a_p = &a;
	insert_value(ll, &a);
	insert_value(ll, &b);
	_callable_print_int_loc(*ll);
	remove_data(ll, &a, compare_int, NULL);
	remove_data(ll, &b, compare_int, NULL);
	insert_value(ll, &a_p);
	_callable_change_int(*ll);
	_callable_change_int(*ll);
	printf("changed value: %d\n", *a_p);
	return 0;
}
