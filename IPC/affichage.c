#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#include "shmem.h"
#include "sharedMemory.h"
#include "semaphore.h"

#define TAILLEMSGTUBE 23

static int PIDECRIVAIN;
static int PIDAGENCE;

void affichageShareMem(SHMEM* pshmem){
  int i;
  printf("\n\nVols Disponibles     | Nombre de places \n\n");
  for(i=0; i<20; i++){
    //printf("affichage sharedmem i: ");
    //printf("%s\n", pshmem[i].destination);
    if(pshmem[i].destination[0] != '?') {
      if (pshmem[i].nbSeats == 0) { // On le supprime de la mémoire
        strcpy(pshmem[i].destination, "????????????????????\0");
        pshmem[i].nbSeats = 0;
      }
      else  {
        printf("%s | %d \n", pshmem[i].destination, pshmem[i].nbSeats);
      }
    }
  }
}

void finAffichage() {

  
  printf("Fermeture Affichage\n");
    
  kill(PIDECRIVAIN, SIGINT); //Fermeture finale de l'écrivain
  kill(PIDAGENCE, SIGINT);  //Femrmeture de l'Agence

  exit(1);
}

void main() {

  int cleMem = 200;
  int cleMutex = 100;
  int idMutex; int idMem;

  signal(SIGINT, finAffichage);

  //Connexion au mutex et à la mémoire des destinations
  if(idMutex = open_semaphore(cleMutex) == -1) {
    perror("create_semaphore"); exit(-1);
  }


  down(idMutex);
  SHMEM *ShrdMem = (SHMEM *)attach_shmem(open_shmem(cleMem,sizeof(SHMEM)*22));
  ShrdMem[4].pid = getpid();
  up(idMutex);
  //printf("Mémoire connectée!\n");
  //Ajout du PID Ecrivain



  while(1) {
    sleep(5);
    down(idMutex);
    PIDECRIVAIN = ShrdMem[0].pid;
    PIDAGENCE = ShrdMem[2].pid;
    affichageShareMem(ShrdMem);
    up(idMutex);
  }

}
