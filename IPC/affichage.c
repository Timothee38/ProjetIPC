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

void finAffichage() {
	
	int idMutexPID = open_semaphore(500);	
	SHMEMPID *ShrdMemPID = (SHMEMPID *)attach_shmem(open_shmem(500,sizeof(SHMEMPID)*5));
	printf("Fermeture Affichage\n");
	kill(ShrdMemPID[2].pid, SIGINT); //Femrmeture de l'Agence
	kill(ShrdMemPID[0].pid, SIGINT);  //Fermeture finale de l'écrivain 
	
    exit(1);
}

void main() {
	
  int cleMem = 200;
  int cleMutex = 100;
  int cleMemPID = 500;
  int cleMutexPID = 600;
  int idMutex; int idMem; int idMutexPID; int idMemPID;

  signal(SIGINT, finAffichage);
	
  //Connexion au mutex et à la mémoire des destinations
  if(idMutex = open_semaphore(cleMutex) == -1) {
    perror("create_semaphore"); exit(-1);
  }

  down(idMutex);
  if((idMem = open_shmem(cleMem, sizeof(SHMEM)*20)) == 0) {
    perror("open_shmem"); exit(-1);
  }
  up(idMutex);
  
  SHMEM *ShrdMem;
  if ((ShrdMem = (SHMEM *)attach_shmem(idMem)) == (SHMEM *)0xffffffffffffffff) { // Permission denied
    perror("attach_shmem"); exit(-1);
  }
  
  
  //Connexion au mutex et à la mémoire des PID
  if(idMutexPID = open_semaphore(cleMutexPID) == -1) {
    perror("create_semaphore PID"); exit(-1);
  }
  
  down(idMutexPID);
  if((idMemPID = open_shmem(cleMemPID, sizeof(SHMEMPID)*5)) == 0) {
    perror("open_shmem PID"); exit(-1);
  }
  up(idMutexPID);

  
  SHMEMPID *ShrdMemPID;
  if ((ShrdMemPID = (SHMEMPID *)attach_shmem(idMemPID)) == (SHMEMPID *)0xffffffffffffffff) { // Permission denied
    perror("attach_shmem PID"); exit(-1);
  }
  
  
  //Ajout du PID Ecrivain
  down(idMutexPID);
  ShrdMemPID[4].pid = getpid();   
  up(idMutexPID);
  

  //printf("pointeur sharedMemory: %p\n", ShrdMem);

  while(1) {
    sleep(5);
    down(idMutex);
    affichageShareMem(ShrdMem);
    up(idMutex);
  }

}
