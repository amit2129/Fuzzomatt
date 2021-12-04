#include <generic_ll.h>

typedef struct _resources {
	LinkedList **ll_array;
} Resources;

typedef struct _stage {
	Resources *resources;
	void (*func_ptr)(LinkedList *args);
	LinkedList *arg_types;
} FuzzerExecutionStage;
