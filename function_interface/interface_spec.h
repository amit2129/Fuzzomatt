
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#define MAP0(f, x, peek, ...) f(x) MAP_NEXT (peek, MAP1) (f, peek, __VA_ARGS__)
#define MAP1(f, x, peek, ...) f(x) MAP_NEXT (peek, MAP0) (f, peek, __VA_ARGS__)
#define MAP(f, ...) EVAL (MAP1 (f, __VA_ARGS__, (), 0))

typedef struct _fuzzer_context FuzzerContext;
typedef struct func_context FunctionContext;

enum argUsage { 
		REG_INPUT=0x1,
		CONSUMED=0x2,
		OUTPUT_PTR=0x4,
};
char *usage_strings[] = {"REG_INPUT", "CONSUMED", "OUTPUT_PTR"};

enum argSource { 
		BAD_VAL=0x1,
		RAND_VAL=0x2,
		RESOURCE_CACHE=0x4,
		RESOURCE_CREATION=0x8};

char *source_strings[] = {"BAD_VAL", "RAND_VAL", "RESOURCE_CACHE", "RESOURCE_CREATION"};
enum resource_type {rt_uint8=1, rt_uint16, rt_int};

char *resource_strings[] = {"uint8_t", "uint16_t", "int *"};

enum value_type {vt_int=1};
char *value_strings[] = {"int"};


int log_2(int val) {
	int i = 0;
	while(val != 1) {
		val /= 2;
		i++;
	}
	return i;
}

void print_usage(char *comp_strings[], int bit_comp, int test_bit) {
	if (bit_comp & test_bit) {
		printf("%s", comp_strings[log_2(test_bit)]);
		if (test_bit * 2 < bit_comp) {
			printf(" | ");
		}
	}
}

void print_possible_usages(int possibleUsages) {
	printf("possibleUsages: ");
	for (int i = REG_INPUT; i < OUTPUT_PTR * 2; i*=2) {
		print_usage(usage_strings, possibleUsages, i);
	}
}
void print_possible_sources(int possible_sources) {
	printf("possibleSources: ");
	for (int i = BAD_VAL; i < RESOURCE_CREATION * 2; i*=2) {
		print_usage(source_strings, possible_sources, i);
	}
}

void print_type(int possibleSources, int type_mem) {
	if (possibleSources & (RESOURCE_CACHE | RESOURCE_CREATION
							)) {
		printf("<%s> ", resource_strings[type_mem-1]);
		(possibleSources & RESOURCE_CREATION) ? printf("(OUT)") : printf("(IN)");
	}
	else {
		printf("<%s>", value_strings[type_mem-1]);
	}
}
typedef struct _arg_type {
	int possibleUsages;
	int possibleSources;
	int resource_id;
	
	enum argUsage usage;
	enum argSource source;
	union {
		enum resource_type r_type;
		enum value_type type;
	} type_union;
	void (*print)(struct _arg_type *arg_type);
} ArgType;

void print_arg_type(ArgType *arg_type) {
	char *prepended_tabs = "\t\t\t";
	printf("\t\targ_type: ");
	print_type(arg_type->possibleSources, arg_type->type_union.r_type);
	printf("\n%s", prepended_tabs);
	print_possible_usages(arg_type->possibleUsages);
	printf("\n");
	printf("%s", prepended_tabs);
	print_possible_sources(arg_type->possibleSources);
	printf("\n");
	printf("%sresource_id: %d\n", prepended_tabs, arg_type->resource_id);
	printf("%susage: %d\n", prepended_tabs, arg_type->usage);
	printf("%ssource: %d\n", prepended_tabs, arg_type->source);
}

typedef struct _arg_type_ll {
	ArgType arg_type;
    struct _arg_type_ll *next;
} ArgTypeLL;


typedef void (*fuzzer_func_t)(FuzzerContext *context, FunctionContext *f_context, int *return_code);

struct func_context {
	ArgTypeLL *arg_types_in_head;
	fuzzer_func_t func;
	char *f_name;
	void (*print)(struct func_context *f_context);
};

void print_func_context(FunctionContext *f_context){
	printf("\t%s\n", f_context->f_name);
	ArgTypeLL *cursor = f_context->arg_types_in_head;
   	while (cursor) {
		cursor->arg_type.print(&(cursor->arg_type));
		cursor = cursor->next;
	}
}

typedef struct general_ll {
	void *data;
	struct general_ll *next;
} LinkedList;

struct _fuzzer_context {
	void (*init)(struct _fuzzer_context *context);
	void (*register_function)(struct _fuzzer_context *context);
	void (*insert_function_context)(struct _fuzzer_context *context, FunctionContext *f_context);
	void (*print_functions)(struct _fuzzer_context *context);
	void *(*get_arg)(ArgTypeLL *type_ll, struct _fuzzer_context *context);
	FunctionContext *(*get_func_context)(int fid, struct _fuzzer_context *context);
	LinkedList **f_context_ll_head;

	LinkedList **resources;
	int (*insert_resource)(void *resource, enum resource_type r_type, struct _fuzzer_context *context);
	int (*remove_resource)(int resource_id, enum resource_type r_type, struct _fuzzer_context *context);
};

LinkedList *get_node(void *data) {
	LinkedList *new_node =(LinkedList *)malloc(sizeof(LinkedList));
	new_node->data = data;
	new_node->next = NULL;
	return new_node;
}

void ll_insert(LinkedList **head_ptr, void *data) {
		if (!*head_ptr) {
			*head_ptr = get_node(data);
		}
		else {
			LinkedList *cursor = *head_ptr;
			while(cursor->next) { cursor = cursor->next;}
			cursor->next = get_node(data);
		}
}


#define REGISTER_FUZZER_FUNCTION_V0(function_name, input_lines, in_args, output_lines) \
	void fuzzer_##function_name(FuzzerContext *context, FunctionContext *f_context, int *return_code) {\
	printf("running function: %s\n", f_context->f_name); \
	ArgTypeLL *arg_type_cursor = f_context->arg_types_in_head; \
	input_lines \
	*return_code = function_name in_args; \
	if (!*return_code) { \
		arg_type_cursor = f_context->arg_types_in_head; \
		output_lines \
	} \
	printf("done running function: %s\n", f_context->f_name); \
}

#define INPUT(type, name) \
		type name = *(type *)context->get_arg(arg_type_cursor, context); \
        arg_type_cursor = arg_type_cursor->next 

#define OUTPUT(type, name) \
		printf("running output var " #name "\n"); \
		if (!arg_type_cursor) { \
			printf("arg_type_cursor was NULL\n");	\
			return;  \
		} \
		printf("arg_type wasn't NULL\n"); \
insert_next_resource: \
		printf("inserting?\n"); \
		if (arg_type_cursor->arg_type.usage & OUTPUT_PTR){ \
			\
			printf("inserting resource\n"); \
			context->insert_resource(name, arg_type_cursor->arg_type.type_union.r_type, context); \
			arg_type_cursor = arg_type_cursor->next; \
		} \
		else { \
			while(arg_type_cursor) { \
				if (arg_type_cursor->arg_type.usage & OUTPUT_PTR) { \
					   	\
					goto insert_next_resource; \
				} \
				arg_type_cursor = arg_type_cursor->next; \
			}\
		}
