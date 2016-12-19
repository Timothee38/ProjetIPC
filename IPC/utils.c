#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <signal.h>
#include <sys/msg.h>

/*
 * Initializes a semaphore, with the id being idsem and the value val.
 */
void initSemaphore(int idsem, int val) {

  if((semctl(idsem, 0, SET_VAL, val)) == -1) {
    printf("Erreur à l'initialisation du sémaphore n°%d\n",idsem);
    exit(1);
  }

}

int initMailbox(key_t cle) {

  int id = 0;
  if((id = msgget(cle, 0666|IPC_CREAT)) == -1) {
    printf("Erreur a la création de la boite aux lettres.\n");
    exit(1);
  }
  return id;

}
