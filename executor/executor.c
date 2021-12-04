

int run_executor(LinkedList *stages) {
	
}


void get_arg(ArgType *arg_type) {
	if (arg_type->resource_id != 0 && arg_type->direction == OUT) {
		return get_resource(arg_type->resource_id);
	}

}


int run_stage(FuzzerExecutionStage *stage) {
	LinkedList *arg_types = stage->arg_types;
	LinkedList *args;
	while (arg_types) {
		ll_insert(&arg_types, get_arg((ArgType *)arg_types.data);
		arg_types = arg_types->next;
	}
}
