CC = gcc

all: clean ecrivain.o affichage.o semaphore.o
	$(CC) ecrivain.o semaphore.o -o IPC

affichage: affichage.o
	$(CC) -c affichage.o semaphore.o -o affichage

ecrivain.o: semaphore.o
	$(CC) -c ecrivain.c -o ecrivain.o

affichage.o: semaphore.o
	$(CC) -c affichage.c -o affichage.o

semaphore.o:
	$(CC) -c  semaphore.c -o  semaphore.o

clean:
	rm -rf *.o IPC
