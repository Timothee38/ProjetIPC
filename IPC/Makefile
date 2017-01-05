CC = gcc

all: ecrivain.o semaphore.o affichage.o agence.o client.o
	$(CC) ecrivain.o semaphore.o -o ECRTIR
	$(CC) affichage.o semaphore.o -o AFF
	$(CC) agence.o semaphore.o -o AGE
	$(CC) client.o semaphore.o -o CLT

affichage: affichage.o semaphore.o
	$(CC) affichage.o semaphore.o -o AFF

client: client.o semaphore.o
	$(CC) client.o semaphore.o -o CLT

agence : agence.o semaphore.o
	$(CC) agence.o semaphore.o -o AGE

agence.o: semaphore.o
	$(CC) -c agence.c -o agence.o

ecrivain.o: semaphore.o
	$(CC) -c ecrivain.c -o ecrivain.o

affichage.o: semaphore.o
	$(CC) -c affichage.c -o affichage.o

client.o: semaphore.o
	$(CC) -c client.c -o client.o

semaphore.o:
	$(CC) -c  semaphore.c -o  semaphore.o

clean:
	rm -rf *.o ECRTIR AGE AFF CLT
