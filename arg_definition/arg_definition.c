#include <arg_definition.h>


int main() {
	
	ArgType a = {.possibleUsages = 0, .possibleSources = 0, .preset_values = NULL, .print=print_arg_type};
	a.print(&a);
	return 0;
}
