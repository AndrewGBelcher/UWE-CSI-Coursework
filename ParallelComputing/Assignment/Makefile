CC=gcc
CFLAGS=-Wall -fopenmp
LDFLAGS=-lz
SOURCES=aes.c main.c
EXECUTABLE=brute
all:
	$(CC) $(CFLAGS) $(SOURCES) $(LDFLAGS) -o $(EXECUTABLE)
clean:
	rm -rf $(EXECUTABLE)