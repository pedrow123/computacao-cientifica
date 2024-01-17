CFLAGS = -O3 -mavx -march=native -lm -Wall

all: ajustePol.o intervaloOp.o matriz.o
	gcc -o ajustePol ajustePol.o matriz.o intervaloOp.o $ -L${LIKWID_LIB} -llikwid $(CFLAGS) 

matriz.o: matriz.c
	gcc $(CFLAGS) -DLIKWID_PERFMON -I${LIKWID_INCLUDE} -c matriz.c

intervaloOp.o: intervaloOp.c
	gcc $(CFLAGS) -DLIKWID_PERFMON -I${LIKWID_INCLUDE} -c intervaloOp.c

ajustePol.o: ajustePol.c
	gcc $(CFLAGS) -DLIKWID_PERFMON -I${LIKWID_INCLUDE} -c ajustePol.c

run: all
	./ajustePol

clean:
	rm -f *.o

purge: clean
	rm -f ajustePol
