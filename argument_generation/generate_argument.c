#include <generate_argument.h>
#include <stdlib.h>

void generate_int(ArgType *arg_type) {
	
}

void generate_argument(ArgType *arg_type) {
	if (resource_id)
	switch(arg_type->source) {
		case BAD_VAL:
			arg_type->data =(int *) malloc(sizeof(int));
			*(int *)(arg_type->data) = rand() % 2 ? 0 : 0xFFFFFFFF;
			break;
		case RAND_VAL:
			arg_type->data =(int *) malloc(sizeof(int));
			*(int *)(arg_type->data) = rand();
			break;
		default:
			break;
	}
}


int main() {
	ArgType a = {.possibleUsages = 0, .possibleSources = 0, .preset_values = NULL, .print=print_arg_type};
	generate_argument(&a);
	printf("")
	return 0;
}
