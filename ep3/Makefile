CFLAGS = -O3 -mavx -march=native

all: interpola.o tabela.o
	gcc -o interpola interpola.o tabela.o -L${LIKWID_LIB} -llikwid $(CFLAGS)

interpola.o: interpola.c
	gcc $(CFLAGS) -DLIKWID_PERFMON -I${LIKWID_INCLUDE} -c interpola.c

tabela.o: tabela.c
	gcc $(CFLAGS) -DLIKWID_PERFMON -I${LIKWID_INCLUDE} -c tabela.c

run: all
	./interpola

clean:
	rm -f *.o

purge: clean
	rm -f interpola
