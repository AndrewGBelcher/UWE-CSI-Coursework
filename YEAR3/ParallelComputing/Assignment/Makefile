CC=gcc
MPICC=mpicc
CFLAGS=-Wall -fopenmp
LDFLAGS=-lz
SOURCES=aes.c mp.c
MPISOURCES=aes.c mpi.c
EXECUTABLE=brute
MPIEXECUTABLE=mpibrute
all:
	$(CC) $(CFLAGS) $(SOURCES) $(LDFLAGS) -o $(EXECUTABLE)
	$(MPICC) $(MPISOURCES) -o $(MPIEXECUTABLE)
clean:
	rm -rf $(EXECUTABLE)
	rm -rf $(MPIEXECUTABLE)