CCFLAGS = -Wall -g

all: MemoryList.o HistList.o ProcessList.o
	gcc $(CCFLAGS) -o shell shell.c MemoryList.o HistList.o ProcessList.o
	./shell

ProcessList: ProcessList.c ProcessList.h
	gcc $(CCFLAGS) -c ProcessList.c

MemoryList: MemoryList.c MemoryList.h
	gcc $(CCFLAGS) -c MemoryList.c 

HistList: HistList.c HistList.h
	gcc $(CCFLAGS) -c HistList.c

clean:
	rm -f *.o 