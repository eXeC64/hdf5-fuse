CC=clang
CFLAGS=-Wall -Wextra -Werror $(shell pkg-config --cflags fuse)
LIBS=$(shell pkg-config --libs fuse)

hdf5: main.c
	$(CC) $(CFLAGS) -o hdf5 main.c $(LIBS)

clean:
	rm -f hdf5
