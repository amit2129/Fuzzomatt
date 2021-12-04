CC=gcc
CFLAGS=-I.
DEPS = generic_ll.h
OBJ = generic_ll.o

%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

generic_ll: $(OBJ)
		$(CC) -o $@ $^ $(CFLAGS)

clean:
		rm -f generic_ll
		rm -rf *.o
