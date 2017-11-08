CC=nvcc
CFLAGS=-O3 -Wall
DEPS=servers.h
OBJ=main.o single.o multi.o gpu.o
LIBS=-lm -pthread

gpu.o: gpu.cu
	nvcc -c gpu.o -O3 -lm -arch=sm_50

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

hellotraveler: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
