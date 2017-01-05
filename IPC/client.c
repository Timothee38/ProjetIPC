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

void finClient() {

  exit(1);

}

void commandeValide(){
  printf("Commande Validée avec succès!\n");
  sleep(9);
  printf("Au revoir!\n");
  sleep(1);
}

void commandeInvalide(){
  printf("Commande non disponible :(\n");
  sleep(9);
  printf("Au revoir!\n");
  sleep(1);
}

void main() {

  signal(SIGUSR1, commandeValide);
  signal(SIGUSR2, commandeInvalide);
  signal(SIGINT, finClient);

  key_t cleBoite = 300;
  int idBoite;

  char dest[20] = "";
  char seats[2] = "";
  int clientPid = (int)getpid();
  char convertedPid[10] = "";
  snprintf(convertedPid, 10, "%d", clientPid);

  // Message a envoyer
  MSG commande;

  //Saisie de la commande

  printf("Entrez votre destination :\n");
  scanf("%s", &dest);

  printf("Combien souhaitez vous de places pour %s :\n", dest);
  scanf("%s", &seats);

  commande.mtype = 1;

  // Remplissage du message
  strcpy(commande.mtext, dest);
  strcat(commande.mtext, " ");
  strcat(commande.mtext, seats);
  strcat(commande.mtext, " ");
  strcat(commande.mtext, convertedPid);
  strcat(commande.mtext, "\0");

  printf("Message envoyé : %s\n", commande.mtext);

  // Création et envoi du message dans la BAL
  if((idBoite = msgget(cleBoite, 0666))==-1){
    perror("Erreur ouverture boite aux lettres");
    exit(-1);
  }

  // Envoi du message dans la BAL
  msgsnd(idBoite, &commande, sizeof(commande.mtext),0);
  pause(10);
}
