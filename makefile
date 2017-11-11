CC=nvcc
CFLAGS=-O3 -ccbin clang-3.8 -arch=sm_50
DEPS=servers.h
OBJ=main.o single.o contdisplay.o
LIBS=-lm -lSDL2

%.o: %.c* $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

hellotraveler: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm *.o hellotraveler
