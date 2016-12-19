CC = gcc

all: clean ecrivain.o semaphore.o
	$(CC) ecrivain.o semaphore.o -o IPC

ecrivain.o: semaphore.o
	$(CC) -c ecrivain.c -o ecrivain.o

semaphore.o:
	$(CC) -c  semaphore.c -o  semaphore.o

clean:
	rm -rf *.o IPC
