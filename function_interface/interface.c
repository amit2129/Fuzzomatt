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



int some_function(int a, char b) {
	printf("%d, %d\n", a, b);
	return 0;
}


#define EXPAND_INPUTS(args) args;

#define EXPAND_OUTPUTS(args) {\
	args \
}


//#define FUZZER_FUNCTION_NAME(function_name)\
 void _fuzzer## _ ## function_name (FuzzerFunctionArgs *inout)

#define FUZZER_EXPOSE_FUNCTION(function_name, in_types, in_args, out_args)\
			 void _fuzzer## _ ## function_name (FuzzerFunctionArgs *inout) {\
			EXPAND_INPUTS(in_types) \
			function_name in_args; \
		}
		//	EXPAND_OUTPUTS(out_args) \
		}



//#define FUZZER_EXPOSE_FUNCTION(fid, return_type, function_name, input_resource, output_resource) ({\
			return_type _fuzzer##fid(input_resource) {


//void function_runner(int fid) {
//	 functions[fid]
//}

//void function_runner(int fid, int length_in, int length_out, ...) {
//	CAST_FID(fid) fp = functions[fid]
//	CALL_FID(fp, args) 
//	void *ret = (void *) &CAST(fid) (UNPACK(FID, in_args));
//	REPACKAGE(FID, ret, in_args);
//}

//#define VA_ARG_CAST(arg_list, type)
//	if type == TYPE_INT {
//	}

//void variadic_function_test(int *types, int length, ...) {
//	va_list ap;
//	int j;
//	double tot = 0;
//	va_start(ap, length);
//	for(j=0; j<count; j++) {
//		tot+=va_arg(ap, double); //Requires the type to cast to. Increments ap to the next argument.
//	}
//    va_end(ap);
//	    return tot/count;
//}
// 


enum directional_type {non_resource, in, out, inout};

typedef struct {
	enum directional_type direction;
	int type_id;
} FuzzerArgType;

#define P99_PROTECT(...) __VA_ARGS__ 
#define REGISTER_FUZZER_FUNCTION(function_name, index, count_in, count_out, array_of_types, array_len) \
	context.functions[index] = _fuzzer## _ ## function_name; \
	context.arg_counts_in[index] = count_in; \
	context.arg_counts_out[index] = count_out; \
	FuzzerArgType args_##function_name[] = array_of_types; \
	for (int i = 0; i < array_len; i++) { \
		context.arg_type_arrays[index][i] = args_##function_name[i]; \
	} \

void copyArray(FuzzerArgType **arg_type_array_ptr, const FuzzerArgType *array, int array_len) {
	
}
typedef void (*fuzzer_function)(FuzzerFunctionArgs *);

void *get_value(FuzzerArgType *arg) {
	void *ret_val;
	switch(arg->direction)
    {
        case '+':
            break;

        case '-':
            break;

        case '*':
            break;

        case '/':
            break;

        // operator doesn't match any case constant +, -, *, /
        default:
            printf("Error! operator is not correct");
    }

	int *ret_int = (int *) malloc(sizeof(int));
	*ret_int = rand();
	ret_val = (void *)ret_int;
	return ret_val;
}


typedef struct _fuzzer_context {
	fuzzer_function *functions;
	uint8_t *arg_counts_in;
	uint8_t *arg_counts_out;
	FuzzerArgType **arg_type_arrays;
	FuzzerFunctionArgs (*build_args)(int index, struct _fuzzer_context *context);
	void (*free_args)(int index, struct _fuzzer_context *context, FuzzerFunctionArgs *args);
	void (*init_context)(int size, struct _fuzzer_context *context);
	void *(*get_value)(FuzzerArgType *arg);
} FuzzerContext;


void free_args(int index, FuzzerContext *context, FuzzerFunctionArgs *args) {
	printf("building\n");
	free(args->in_args);
	free(args->out_args);
}

FuzzerFunctionArgs build_args(int index, FuzzerContext *context) {
	FuzzerFunctionArgs ret_args;
	ret_args.in_args = (void **) malloc(sizeof(void *) * context->arg_counts_in[index]);
	ret_args.out_args = (void **) malloc(sizeof(void *) * context->arg_counts_out[index]);
	FuzzerArgType *cursor = context->arg_type_arrays[index];

	for (int i = 0; i < context->arg_counts_in[index]; i++) {
		ret_args.in_args[i] = context->get_value(context->arg_type_arrays[index] + i);
		printf("int_val: %d\n",*(int *)ret_args.in_args[i]);
	}
	for (int i = 0; i < context->arg_counts_out[index]; i++) {
		ret_args.out_args[i] = context->get_value(context->arg_type_arrays[index] + i);
	}


	return ret_args;
}

void init_context(int size, FuzzerContext *context) {
	context->build_args = &build_args;
	context->get_value = &get_value;
	context->free_args = &free_args;
	context->functions = (fuzzer_function *) malloc(sizeof(fuzzer_function) * size);
	context->arg_counts_in = (uint8_t *) malloc(sizeof(uint8_t) * size);
	context->arg_counts_out = (uint8_t *) malloc(sizeof(uint8_t) * size);
	context->arg_type_arrays = (FuzzerArgType **)malloc(sizeof(FuzzerArgType *) * size);
	for (int i =0; i < size; i++) {
		context->functions[i] = NULL;
		context->arg_type_arrays[i] = (FuzzerArgType *) malloc(sizeof(FuzzerArgType) * size);
	}
}


//typedef struct {
//	void **in_args;
//	void **out_args;
//	int return_code;
//} FuzzerFunctionArgs;


FUZZER_EXPOSE_FUNCTION(some_function,int a=*(int*)inout->in_args[0]; int b=*(int*)inout->in_args[1], (a, b), ())
int main() {
	srand(time(NULL));
	FuzzerContext context;
	context.init_context = &init_context;
	context.init_context(10, &context);
	REGISTER_FUZZER_FUNCTION(some_function, 1, 2, 0, P99_PROTECT({1, 2, 3}), 3);
	FuzzerFunctionArgs args2 = context.build_args(1, &context);
	context.functions[1](&args2);
//	context.free_args(1, &context);
	
}


