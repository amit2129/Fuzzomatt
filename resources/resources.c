#include <resources.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

static long unsigned counter;

int insert_resource(ResourceContext *resources, Resource *resource) {
	LinkedList *resources_ll = resources->resource_values;
	ResourceType *rt_p;
	while(resources_ll) {
			rt_p = resources_ll->data;
			if (rt_p->type == resource->type) {
				resource->resource_id = ++counter;
				printf("inserted resource: %d with id: %ld\n", resource->type, resource->resource_id);
				ll_insert_value(&(rt_p->resource_elements), resource);
				return resource->resource_id;
			}
			resources_ll = resources_ll->next;
	}
	rt_p = (ResourceType *) malloc(sizeof(ResourceType));
	rt_p->resource_elements = NULL;
	rt_p->type = resource->type;
	resource->resource_id = ++counter;
	printf("inserted resource: %d with id: %ld\n", resource->type, resource->resource_id);
	ll_insert_value(&(rt_p->resource_elements), resource);
	ll_insert_value(&(resources->resource_values), rt_p);
	return resource->resource_id;
}
void print_resource(void *data) {
	printf("resource ptr: %p\n", data);
	if (data) {
		Resource *r_p = data;
		printf("\ttype: %d\n", r_p->type);
		printf("\tresource_value: %d\n", *(int *)r_p->resource_value);
		printf("\tresource_id: %ld\n", r_p->resource_id);
	}
}


int get_resource(ResourceContext *resources, enum resource_type type, int resource_id, Resource **resource_out) {
	LinkedList *resources_ll = resources->resource_values;
	printf("get resource: %d, id:%d\n", type, resource_id);
	printf("resources_ll: %p\n", resources_ll);
	*resource_out = NULL;
	while(resources_ll) {
			ResourceType *rt_p = (ResourceType *)resources_ll->data;
			if (!type || type == rt_p->type) {
				LinkedList *resource_elements = rt_p->resource_elements;
				while(resource_elements) {
					Resource *resource = (Resource *)resource_elements->data;
					if (!resource_id || resource_id == resource->resource_id) {
						*resource_out = resource;
						return 0;
					}
					resource_elements = resource_elements->next;
				}
			}
			resources_ll = resources_ll->next;
	}
	return -EINVAL;
}

int compare_resource_and_type(LinkedList *a, LinkedList *b) {
	Resource *res = (Resource *)a->data;
	enum resource_type type = *(enum resource_type *)b->data;
	return res->type == type;
}

int compare_resource_type_and_type(LinkedList *a, LinkedList *b) {
	ResourceType *res = (ResourceType *)a->data;
	enum resource_type type = *(enum resource_type *)b->data;
	return res->type == type;
}

int remove_resource(ResourceContext *resources, enum resource_type type, int resource_id) {
	printf("remove resource: %d, id:%d\n", type, resource_id);
	LinkedList *resources_ll = resources->resource_values;
	while(resources_ll) {
			ResourceType *rt_p = resources_ll->data;
			if (rt_p->type == type) {
				LinkedList *resource_elements = rt_p->resource_elements;
				int ret = remove_data(&rt_p->resource_elements, &type, compare_resource_and_type, NULL);
				printf("rt_p->res_elem: %p\n", rt_p->resource_elements);
				if (!rt_p->resource_elements) {
					remove_data(&(resources->resource_values), &type, compare_resource_type_and_type, NULL);
				}
				return ret;
			}
	}
	return -EINVAL;
}


void test_resources1() {
	ResourceContext rc;
	initResourceContext(&rc);
	int a = 5;
	Resource resource1 = {.resource_value=&a, .type=rt_int};
	int b = 8432432;
	Resource resource2 = {.resource_value=&b, .type=rt_int};
	printf("inserting resource\n");
	rc.insert_resource(&rc, &resource1);
	rc.insert_resource(&rc, &resource2);
	Resource *r_out_p;
	rc.get_resource(&rc, rt_int, 2, &r_out_p);
	
	Resource r_out = *r_out_p;
   	printf("r_out:\n\tresource_id %ld\n\tvalue:%d\n\ttype:%d\n", r_out.resource_id, *(int *)r_out.resource_value, r_out.type);	
	rc.remove_resource(&rc, rt_int, 1);
	rc.remove_resource(&rc, rt_int, 2);
	rc.get_resource(&rc, rt_int, 1, &r_out_p);
	printf("r_out_p: %p\n", r_out_p);
	printf("resource2: %d\n", resource2.type);
	rc.insert_resource(&rc, &resource2);
	rc.get_resource(&rc, rt_int, 3, &r_out_p);
	if (!r_out_p) {
		return;
	}
	r_out = *r_out_p;
   	printf("r_out:\n\tresource_id %ld\n\tvalue:%d\n\ttype:%d\n", r_out.resource_id, *(int *)r_out.resource_value, r_out.type);	
}

void test_resources2() {
	ResourceContext rc;
	initResourceContext(&rc);
	int a = 5;
	Resource resource1 = {.resource_value=&a, .type=rt_int};
	char b = 'a';
	Resource resource2 = {.resource_value=&b, .type=rt_char};
	printf("inserting resource\n");
	int id_a = rc.insert_resource(&rc, &resource1);
	int id_b = rc.insert_resource(&rc, &resource2);
	Resource *r_out_p;
	rc.get_resource(&rc, rt_int, 0, &r_out_p);
	
	Resource r_out = *r_out_p;
   	printf("r_out:\n\tresource_id %ld\n\tvalue:%d\n\ttype:%d\n", r_out.resource_id, *(int *)r_out.resource_value, r_out.type);	

	rc.get_resource(&rc, rt_char, 0, &r_out_p);
	r_out = *r_out_p;
   	printf("r_out:\n\tresource_id %ld\n\tvalue:%c\n\ttype:%d\n", r_out.resource_id, *(char *)r_out.resource_value, r_out.type);	
}

//int main() {
//	test_resources1();
//	test_resources2();
//	return 0;
//}
