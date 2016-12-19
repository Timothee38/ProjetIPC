#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "shmem.h"
#include "sharedMemory.h"
#include "semaphore.h"

#define TAILLEMSGTUBE 23

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

  if(idMem = open_shmem(cleMem, sizeof(23)*20) == -1) {
    perror("open_shmem"); exit(-1);
  }

  SHMEM *ShrdMem = (SHMEM *)attach_shmem(idMem);

  while(1) {
    sleep(5);
    down(idMutex);
    

  }

}
