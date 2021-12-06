CC=gcc
CFLAGS=-I. -I../shared/ -I../arg_definition/
DEPS = generate_argument.h arg_definition.h
OBJ = generate_argument.o

%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

generate_argument: $(OBJ)
		$(CC) -o $@ $^ $(CFLAGS)

clean:
		rm -f generate_argument
		rm -rf *.o
		rm -rf *.h.*
