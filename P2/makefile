CCFLAGS = -Wall -g

all: clean list.o array.o
	gcc $(CCFLAGS) -o shell shell.c list.o array.o

list.o: list.c list.h
	gcc $(CCFLAGS) -c list.c

array.o: array.c 
	gcc $(CCFLAGS) -c array.c

clean:
	rm -f *.o shell