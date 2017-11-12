CC=nvcc
CFLAGS=-O3  -arch=sm_50
DEPS=servers.h
OBJ=main.o single.o contdisplay.o multi.o gpu.o
LIBS=-lm -lSDL2 -Xcompiler="-pthread"

%.o: %.c* $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

hellotraveler: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm *.o hellotraveler
