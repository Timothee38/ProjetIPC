#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "shmem.h"
#include "semaphore.h"
#include "sharedMemory.h"
#include "message.h"

MSG *initMSG() {
    MSG* ret = (MSG*)malloc(sizeof(MSG));
    ret->messageType=2;
    (ret->mess).pid=getpid();
    return ret;
}

void finClient() {

  // TODO: Detachemenet BAL + Fermeture process

  exit(1);

}

void commandeValide(){
  printf("Commande Validée avec succès!\n");
}

void commandeInvalide(){
	printf("Commande non disponible :(\n");
}

void main() {

  signal(SIGUSR1, commandeValide);
  signal(SIGUSR2, commandeInvalide);
  signal(SIGINT, finClient);

  int i;
  int clientPid = getpid();

  MSG* commande = initMSG();

  //Saisie de la commande
  printf("Entrez votre destination :\n");
  scanf("%s", &commande->mess.dest);
  printf("Combien souhaitez vous de places pour %s :\n", (commande->mess).dest);
  scanf("%s", &commande->mess.seats);

  printf("%d = %d ?\n", getpid(), (commande->mess).pid);

  // Création et envoi du message dans la BAL
  

  // Liberation mémoire de commande
  free(commande);


}
