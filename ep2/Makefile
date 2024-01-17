CFLAGS = -O3 -mavx -march=native

all: perfEG.o matriz.o
	gcc -o perfEG matriz.o perfEG.o -L${LIKWID_LIB} -llikwid $(CFLAGS)

matriz.o: matriz.c
	gcc $(CFLAGS) -DLIKWID_PERFMON -I${LIKWID_INCLUDE} -c matriz.c

perfEG.o: perfEG.c
	gcc $(CFLAGS) -DLIKWID_PERFMON -I${LIKWID_INCLUDE} -c perfEG.c

run: all
	./perfEG

clean:
	rm -f *.o

purge: clean
	rm -f perfEG
