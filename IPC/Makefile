CC = gcc

all: ecrivain.o semaphore.o affichage.o agence.o
	$(CC) ecrivain.o semaphore.o -o IPC

affichage: affichage.o semaphore.o
	$(CC) affichage.o semaphore.o -o AFF

agence : agence.o semaphore.o
	$(CC) agence.o semaphore.o -o AGE

agence.o: semaphore.o
	$(CC) -c agence.c -o agence.o

ecrivain.o: semaphore.o
	$(CC) -c ecrivain.c -o ecrivain.o

affichage.o: semaphore.o
	$(CC) -c affichage.c -o affichage.o

semaphore.o:
	$(CC) -c  semaphore.c -o  semaphore.o

clean:
	rm -rf *.o IPC AGE AFF
