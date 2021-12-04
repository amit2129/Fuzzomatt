CC=gcc
CFLAGS=-I. -I ../shared
DEPS = generic_ll.h
OBJ = generic_function_call.o ../shared/generic_ll.o

%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

generic_function_call: $(OBJ)
		$(CC) -o $@ $^ $(CFLAGS)

clean:
		rm -f generic_function_call
		rm -rf *.o
