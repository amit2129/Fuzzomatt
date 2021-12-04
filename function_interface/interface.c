#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>


//#define FUZZED_FUNCTION(function, input_args, output_args) { }

typedef struct {
	void **in_args;
	void **out_args;
	int return_code;
} FuzzerFunctionArgs;

int some_function(int a, int b) {
	printf("%d, %d\n", a, b);	
	return 0;
}

int get_int_function(int *a) {
	*a = rand();
	return 0;
}

typedef struct some_struct_example {
		int some_val;
		int other_val;
} some_struct_example;

int init_struct_function(struct some_struct_example *example_struct) {
		example_struct->some_val = 21;
		example_struct->other_val = 29;
}

#define EXPAND_INPUTS(args) args;

#define EXPAND_OUTPUTS(args) {\
	args \
}


//#define FUZZER_FUNCTION_NAME(function_name)\
 void _fuzzer## _ ## function_name (FuzzerFunctionArgs *inout)

#define FUZZER_EXPOSE_FUNCTION(function_name, in_types, out_types, in_args)\
			 void _fuzzer## _ ## function_name (FuzzerFunctionArgs *inout) {\
			in_types; \
			inout->return_code = function_name in_args; \
			out_types; \
		}
		//	EXPAND_OUTPUTS(out_args) \
		}



//#define FUZZER_EXPOSE_FUNCTION(fid, return_type, function_name, input_resource, output_resource) ({\
			return_type _fuzzer##fid(input_resource) {

#define REGISTER_FUZZER_RESOURCE(resource_name) resource_name

enum directionalType {non_resource, in, out, inout};
enum resourceType { resource_int=1 , resource_intp };

typedef struct {
	enum directionalType direction;
	enum resourceType resource_type;
} FuzzerArgType;

#define P99_PROTECT(...) __VA_ARGS__ 
#define REGISTER_FUZZER_FUNCTION(context, function_name, index, count_in, count_out, array_of_types_in, array_of_types_out,array_len_in, array_len_out) \
	context->functions[index] = _fuzzer## _ ## function_name; \
	context->arg_counts_in[index] = count_in; \
	context->arg_counts_out[index] = count_out; \
	FuzzerArgType args_##function_name##_in[] = array_of_types_in; \
	context->arg_type_arrays[index].arg_types_in = (FuzzerArgType *) malloc(sizeof(FuzzerArgType) * count_in); \
	context->arg_type_arrays[index].arg_types_out = (FuzzerArgType *) malloc(sizeof(FuzzerArgType) * count_out); \
	copyArray(context->arg_type_arrays[index].arg_types_in, args_##function_name##_in, array_len_in); \
	FuzzerArgType args_##function_name##_out[] = array_of_types_out; \
	copyArray(context->arg_type_arrays[index].arg_types_out, args_##function_name##_out, array_len_out); \
	index++;

void copyArray(FuzzerArgType *arg_type_array_ptr, FuzzerArgType *array, int array_len) {
	for (int i = 0; i < array_len; i++) {
		arg_type_array_ptr[i] = array[i]; 
	}
}

typedef void (*fuzzer_function)(FuzzerFunctionArgs *);

void *get_value(FuzzerArgType *arg) {
	void *ret_val;
	switch(arg->resource_type)
    {
        case resource_int: {
			int *ret_int = (int *) malloc(sizeof(int));
			*ret_int = rand();
			ret_val = (void *)ret_int;
            break;
		}
        case resource_intp: {
			int *ret_int = (int *) malloc(sizeof(int));
			ret_val = (void *)ret_int;
            break;
		}

        default:
            printf("Error! operator is not correct");
    }

	return ret_val;
}


typedef struct _linked_list {
	int data;
	struct _linked_list *next;
} LinkedList;


typedef struct {	
	FuzzerArgType *arg_types_in;
	FuzzerArgType *arg_types_out;
} ArgTypePair;

typedef struct _fuzzer_context {
	fuzzer_function *functions;
	uint8_t *arg_counts_in;
	uint8_t *arg_counts_out;
	ArgTypePair *arg_type_arrays;
	FuzzerFunctionArgs (*build_args)(int index, struct _fuzzer_context *context);
	void (*free_args)(int index, struct _fuzzer_context *context, FuzzerFunctionArgs *args);
	void (*init_context)(int size, struct _fuzzer_context *context);
	void *(*get_value)(FuzzerArgType *arg);
	LinkedList **func_seq_head;
	void (*append_function)(int fid, struct _fuzzer_context *context);
	void (*run_functions)(struct _fuzzer_context *context);
} FuzzerContext;

void append_function(int fid, FuzzerContext *context) {
		LinkedList *cursor = *context->func_seq_head;
		LinkedList *new_val = (LinkedList *) malloc(sizeof(LinkedList));
		new_val->data = fid;
		new_val->next = NULL;

		if (!cursor) {
			*context->func_seq_head = new_val;
		}
		else {
			while (cursor->next) {
				cursor = cursor->next;
			}
			cursor->next = new_val;
		}

}


void free_args(int index, FuzzerContext *context, FuzzerFunctionArgs *args) {
	for (int i = 0; i < context->arg_counts_in[index]; i++){
			free(args->in_args[i]);
	}
	for (int i = 0; i < context->arg_counts_out[index]; i++) {
			free(args->out_args[i]);
	}

	free(args->in_args);
	free(args->out_args);
}

FuzzerFunctionArgs build_args(int index, FuzzerContext *context) {
	FuzzerFunctionArgs ret_args;
	ret_args.in_args = (void **) malloc(sizeof(void *) * context->arg_counts_in[index]);
	ret_args.out_args = (void **) malloc(sizeof(void *) * context->arg_counts_out[index]);

	for (int i = 0; i < context->arg_counts_in[index]; i++) {
		ret_args.in_args[i] = context->get_value(context->arg_type_arrays[index].arg_types_in + i);
	}

	for (int i = 0; i < context->arg_counts_out[index]; i++) {
		ret_args.out_args[i] = context->get_value(context->arg_type_arrays[index].arg_types_out + i);
	}

	return ret_args;
}

void run_functions(FuzzerContext *context) {
	LinkedList *cursor = *context->func_seq_head;
	while (cursor) {
		int f_index = cursor->data;
		printf("running function: %d\n", f_index);
		FuzzerFunctionArgs f_args = context->build_args(f_index, context);
		context->functions[f_index](&f_args);
		if (context->arg_counts_out[f_index]) {
			printf("out int: %d\n",*(int *)f_args.out_args[0]);
		}
		printf("return code: %d\n", f_args.return_code);
		context->free_args(f_index, context, &f_args);
		cursor = cursor->next;
	}
}

void init_context(int size, FuzzerContext *context) {
	context->build_args = &build_args;
	context->run_functions = &run_functions;
	context->append_function = &append_function;
	context->get_value = &get_value;
	context->free_args = &free_args;
	context->func_seq_head = (LinkedList **) malloc(sizeof(LinkedList *));
	*context->func_seq_head = NULL;
	context->functions = (fuzzer_function *) malloc(sizeof(fuzzer_function) * size);
	context->arg_counts_in = (uint8_t *) malloc(sizeof(uint8_t) * size);
	context->arg_counts_out = (uint8_t *) malloc(sizeof(uint8_t) * size);
	context->arg_type_arrays = (ArgTypePair *)malloc(sizeof(ArgTypePair) * size);
	for (int i =0; i < size; i++) {
		context->functions[i] = NULL;
	}
}

FUZZER_EXPOSE_FUNCTION(some_function, int a=*(int*)inout->in_args[0]; int b=*(int*)inout->in_args[1], , (a, b))
FUZZER_EXPOSE_FUNCTION(get_int_function, int *a=(int*)inout->in_args[0];,*(int *)inout->out_args[0] = *(int *)a; , (a));
FUZZER_EXPOSE_FUNCTION(init_struct_function, some_struct_example **a = *(some_struct_example *)inout->in_args[0] = *(some_struct_example *)a;,  , (a));

void register_functions(FuzzerContext *context) {
	int fid = 0;
	REGISTER_FUZZER_FUNCTION(context, some_function, 
					fid, 
					2, 
					0, 
					P99_PROTECT({
									(FuzzerArgType){.direction=in, .resource_type=resource_int},
									(FuzzerArgType){.direction=in, .resource_type=resource_int}
								}),
					P99_PROTECT({
									(FuzzerArgType){.direction=out, .resource_type=0}
								}),
				   	2,
					0);

	REGISTER_FUZZER_FUNCTION(context, get_int_function, 
					fid, 
					1, 
					1, 
					P99_PROTECT({
									(FuzzerArgType){.direction=in, .resource_type=resource_intp}
								});,
					P99_PROTECT({
									(FuzzerArgType){.direction=out, .resource_type=resource_intp}
								});,
				   	1,
					1)
}

void init_fuzzer() {
	FuzzerContext context;
	context.init_context = &init_context;
	context.init_context(10, &context);

	register_functions(&context);
	context.append_function(0, &context);
	context.append_function(1, &context);
	context.run_functions(&context);	
}

int main() {
	srand(time(NULL));
	init_fuzzer();
}


