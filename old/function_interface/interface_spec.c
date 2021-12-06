#include "interface_spec.h"


int create_int(int *b) {
	printf("b: %p\n", b);
	*b = 2129;
	return 0;
}

int some_function(int a, int b) {
	printf("%d, %d\n", a, b);
	return 0;
}

REGISTER_FUZZER_FUNCTION_V0(some_function, INPUT(int, a); INPUT(int, b);, (a,b), )
REGISTER_FUZZER_FUNCTION_V0(create_int, INPUT(int*, a);, (a), OUTPUT(int*, a))


void fuzzer_register_functions(FuzzerContext *context) {
	ArgTypeLL *cursor;
	FunctionContext *function_context =(FunctionContext *) malloc(sizeof(FunctionContext));
	function_context->func = fuzzer_some_function;
	function_context->print = &print_func_context; 
	function_context->arg_types_in_head =(ArgTypeLL *) malloc(sizeof(ArgTypeLL));
	char *name_fuzzer_some_function = "fuzzer_some_function";
	function_context->f_name = (char *) malloc(strlen(name_fuzzer_some_function) + 1);
	strcpy(function_context->f_name, name_fuzzer_some_function);
	cursor = function_context->arg_types_in_head;
	cursor->next = NULL;
	cursor->arg_type = (ArgType){.possibleUsages=REG_INPUT, .possibleSources=RESOURCE_CACHE, .type_union = rt_int, .print=&print_arg_type};
	cursor->next = (ArgTypeLL *) malloc(sizeof(ArgTypeLL));
	cursor = cursor->next;
	cursor->next = NULL;
	cursor->arg_type = (ArgType){.possibleUsages=REG_INPUT, .possibleSources=RESOURCE_CACHE, .type_union = rt_int, .print=&print_arg_type};

	context->insert_function_context(context, function_context);

	function_context =(FunctionContext *) malloc(sizeof(FunctionContext));
	function_context->func = fuzzer_create_int;
	function_context->print = &print_func_context; 
	function_context->arg_types_in_head =(ArgTypeLL *) malloc(sizeof(ArgTypeLL));
	name_fuzzer_some_function = "fuzzer_create_int";
	free(function_context->f_name);
	function_context->f_name = (char *) malloc(strlen(name_fuzzer_some_function) + 1);
	strcpy(function_context->f_name, name_fuzzer_some_function);
	cursor = function_context->arg_types_in_head;
	cursor->next = NULL;
	cursor->arg_type = (ArgType){.possibleUsages=OUTPUT_PTR, .possibleSources=RESOURCE_CREATION, .type_union=rt_int, .print=&print_arg_type};
	context->insert_function_context(context, function_context);
}

void *get_arg(ArgTypeLL *type_ll, FuzzerContext *context) {
	printf("getting_arg: \n");
	type_ll->arg_type.print(&type_ll->arg_type);
	 if (type_ll->arg_type.possibleSources & RESOURCE_CREATION ) {
		printf("<resource creation>: \n");
		switch(type_ll->arg_type.type_union.r_type) {
				case rt_int: {
					int **tmp =(int **) malloc(sizeof(int *));
					*tmp = (int *) malloc(sizeof(int));
					type_ll->arg_type.usage = OUTPUT_PTR;
					return (void *)tmp;
				}
				default:
					break;
		}
	 }		 
		switch(type_ll->arg_type.type_union.type) {
				case vt_int: {
					int *tmp = malloc (sizeof(int));
					*tmp = rand();
					return (void *)tmp;
					break;
				}
				default:
					break;
		}
		printf("returned NULL\n");
		return NULL;
}


int insert_resource(void *value, enum resource_type type, FuzzerContext *context) {
	context->resource
	return 0;
}

void insert_function_context(FuzzerContext *context, FunctionContext *f_context) {
	ll_insert(context->f_context_ll_head, (void *)f_context);
}

void print_functions(FuzzerContext *context) {
	printf("registered functions:\n");
	LinkedList *cursor = *context->f_context_ll_head;
	while(cursor) {
			FunctionContext *f_context =(FunctionContext *) cursor->data;
			f_context->print(f_context);
			cursor = cursor->next;
	}
}

void init_context(FuzzerContext *context) {
	context->f_context_ll_head = (LinkedList **) malloc(sizeof(LinkedList *));
	*context->f_context_ll_head = NULL;
	context->resources = (LinkedList **) malloc(sizeof(LinkedList *));
	*context->resources = (LinkedList *)malloc(sizeof(LinkedList));

	for (int i=rt_uint8; i < rt_int; i++) {
		LinkedList **new_r_ll = (LinkedList **) malloc(sizeof(LinkedList *));
		*new_r_ll = NULL;
		ll_insert(context->resources, (void *)new_r_ll);
	   	
	}
	context->register_function = &fuzzer_register_functions; 
	context->insert_function_context = insert_function_context;
	context->print_functions = print_functions;
	context->get_arg = get_arg;
	context->insert_resource = &insert_resource;
	
}

int main() {
	srand(time(NULL));
	FuzzerContext context;
	context.init = &init_context;
	context.init(&context);
	fuzzer_register_functions(&context);
	context.print_functions(&context);
	int return_code;
	FunctionContext *f_c = (FunctionContext *)((*context.f_context_ll_head)->next->data);
	f_c->func(&context, f_c, &return_code);
	printf("get_func\n");
	printf("run_func\n");
//	f_c = (FunctionContext *)((*context.f_context_ll_head)->data);
//	f_c->func(&context, f_c, &return_code);
	return 0;
}
