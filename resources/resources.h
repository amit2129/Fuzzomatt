#ifndef _RESOURCES_H
#define _RESOURCES_H

#include <generic_ll.h>
#include <stddef.h>

enum resource_type { rt_int=1, rt_char };

typedef struct _resource {
	long unsigned resource_id;
	void *resource_value;
	enum resource_type type;
} Resource;

typedef struct _resource_type_ll {
	LinkedList *resource_elements;
	enum resource_type type;
} ResourceType;

typedef struct _resources {
	LinkedList *resource_values;
	int (*insert_resource)(struct _resources *resources, Resource *resource);
	int (*get_resource)(struct _resources *resources, enum resource_type type, int resource_id, Resource **out_resource);
	int (*remove_resource)(struct _resources *resources, enum resource_type type, int resource_id);
} ResourceContext;

int insert_resource(ResourceContext *resources, Resource *resource);

int get_resource(ResourceContext *resources, enum resource_type type, int resource_id, Resource **out_resource);

int remove_resource(ResourceContext *resources, enum resource_type type, int resource_id);

void initResourceContext(ResourceContext *r_context) {
	r_context->resource_values = NULL;
	r_context->insert_resource = insert_resource;
	r_context->get_resource = get_resource;
	r_context->remove_resource = remove_resource;
}

#endif
