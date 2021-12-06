#ifndef _ARG_DEFINITION_H
#define _ARG_DEFINITION_H

#include <stdio.h>
#include <generic_ll.h>

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

enum value_type {vt_int=0x1};
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

typedef struct _arg_type ArgType;

struct _arg_type {
	int possibleUsages;
	int possibleSources;
	int resource_id;
	
	enum argUsage usage;
	enum argSource source;
	union {
		enum resource_type r_type;
		enum value_type type;
	} type_union;

	void *data;

	LinkedList *preset_values;
	void (*print)(struct _arg_type *arg_type);
};

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

#endif
