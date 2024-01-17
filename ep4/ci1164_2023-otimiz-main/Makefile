CFLAGS = -O3 -mavx -march=native

all: matmult.o matriz.o
	gcc -o matmult matmult.o matriz.o -L${LIKWID_LIB} -llikwid $(CFLAGS)

matmult.o: matmult.c
	gcc $(CFLAGS) -DLIKWID_PERFMON -I${LIKWID_INCLUDE} -c matmult.c

matriz.o: matriz.c
	gcc $(CFLAGS) -DLIKWID_PERFMON -I${LIKWID_INCLUDE} -c matriz.c

run: all
	./matmult

clean:
	rm -f *.o

purge: clean
	rm -f matmult
