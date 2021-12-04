CC=gcc
CFLAGS=-I. -I ../shared
DEPS = resources.h
OBJ = resources.o ../shared/generic_ll.o

%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

resources: $(OBJ)
		$(CC) -o $@ $^ $(CFLAGS)

clean:
		rm -f resources
		rm -rf *.o
