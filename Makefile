CC=clang
CFLAGS=-Wall -Wextra $(shell pkg-config --cflags fuse)
LIBS=-lhdf5 $(shell pkg-config --libs fuse)

hdf5: main.c
	$(CC) $(CFLAGS) -o hdf5 main.c $(LIBS)

clean:
	rm -f hdf5
