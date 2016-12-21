#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#include "shmem.h"
#include "sharedMemory.h"
#include "semaphore.h"

#define TAILLEMSGTUBE 23

void affichageShareMem(SHMEM* pshmem){
  int i;
  printf("Vols Disponibles     | Nombre de places \n");
  for(i=0; i<20; i++){
    //printf("affichage sharedmem i: ");
    //printf("%s\n", pshmem[i].destination);
      if(pshmem[i].destination[0] != '?') {
        if (pshmem[i].nbSeats == 0) { // We remove it from the memory
          printf("If afficheShareMem\n");
          strcpy(pshmem[i].destination, "????????????????????\0");
          pshmem[i].nbSeats = 0;
        }
        else  {
          printf("%s | %d \n", pshmem[i].destination, pshmem[i].nbSeats);
        }
      }
  }
}

void fin() {
  exit(1);
}

void main() {

  int cleMutex = 100;
  int cleMem = 200;

  int idMutex; int idMem;

  signal(SIGINT, fin);

  if(idMutex = open_semaphore(cleMutex) == -1) {
    perror("create_semaphore"); exit(-1);
  }

  // Il faut le mutex???
  down(idMutex);
  if((idMem = open_shmem(cleMem, sizeof(SHMEM)*20)) == 0) {
    perror("open_shmem"); exit(-1);
  }
  up(idMutex);

  //printf("%d\n", idMem);
  SHMEM *ShrdMem;
  if ((ShrdMem = (SHMEM *)attach_shmem(idMem)) == (SHMEM *)0xffffffffffffffff) { // Permission denied
    perror("attach_shmem"); exit(-1);
  }

  //printf("pointeur sharedMemory: %p\n", ShrdMem);

  while(1) {
    printf("entree while\n");
    sleep(5);
    down(idMutex);
    affichageShareMem(ShrdMem);
    up(idMutex);
  }

}
