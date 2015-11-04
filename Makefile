.PHONY: clean

CFLAGS = -Wall -Wextra $(shell pkg-config --cflags fuse)
LDLIBS =-lhdf5 $(shell pkg-config --libs fuse)
TARGET = hdf5
SOURCES = main.c

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

clean:
	$(RM) $(TARGET)
