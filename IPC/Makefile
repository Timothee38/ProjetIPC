CC = gcc

all: clean ecrivain.o
	$(CC) ecrivain.o -o IPC

ecrivain.o:
	$(CC) -c ecrivain.c -o ecrivain.o

clean:
	rm -rf *.o IPC
