all: integra.o
	gcc -O3 integra.o -o integra -lm  

integra.o: integra.c
	gcc -O3 -c integra.c 

clean:
	rm -f *.o

purge: clean
	rm -f integra
