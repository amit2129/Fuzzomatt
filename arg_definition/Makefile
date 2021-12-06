CC=gcc
CFLAGS=-I. -I../shared/
DEPS = arg_definition.h generic_ll.h
OBJ = arg_definition.o

%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

arg_definition: $(OBJ)
		$(CC) -o $@ $^ $(CFLAGS)

clean:
		rm -f arg_definition
		rm -rf *.o
		rm -rf *.h.*
