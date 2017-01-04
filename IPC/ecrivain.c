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

#define TAILLEMSGTUBE 23

void finEcrivain(){
	
	static int premierPassage = 0;
	
	if(premierPassage == 0){
		
		int idMutex = open_semaphore(100);	
		SHMEM *ShrdMem = (SHMEM *)attach_shmem(open_shmem(200,sizeof(SHMEM)*22));
		
		printf("Lancement de la fermeture\n");
		signal(SIGINT, finEcrivain);
		premierPassage = 1;
		
		kill(ShrdMem[1].pid, SIGKILL); //Fermeture du fils
		kill(ShrdMem[4].pid, SIGINT); //Femrmeture de l'affichage
		
	}else{
		
	  printf("Extinction de l'écrivain\n");
	  
	  //Supression des mémoires partagées
	  int idMem;
	  if (idMem = open_shmem(200, sizeof(SHMEM)*22) == -1) {
	    perror("fin_open_shmem"); exit(-1);
	  }
	  remove_shmem(idMem);
	  
	  //Supression des sémaphores
	  remove_semaphore(open_semaphore(100));
	  remove_semaphore(open_semaphore(400));
	  
	  //THE END
	  exit(1);
	}
}

void lecture() { /* This function does nothing, it's just here to trigger the end of the father's pause. */  }


void afficheShareMem(SHMEM* pshmem){
  int i;
  printf("Destination          | nbSeats \n");
  for(i=0; i<20; i++){
      printf("%s | %d \n", pshmem[i].destination, pshmem[i].nbSeats);
    }
}

void initShareMem(SHMEM* pshmem) {
  int i;
  for(i = 0; i<20; i++) {
    strcpy(pshmem[i].destination, "????????????????????\0");
    pshmem[i].nbSeats = 0;
  }
}

void addShareMem(SHMEM* pshmem, char *destseats){
  char dest[21] = ""; char seats[3]= ""; int i;
  for(i = 0; i < 20; i++) {
      dest[i] = destseats[i];
  }
  dest[20] = '\0';
  seats[0] = destseats[21];
  seats[1] = destseats[22];
  seats[2] = '\0';
      printf("dest I : %s\n", dest);
      printf("seats I : %s\n", seats);
    i = 0;

      while((pshmem[i].destination[0]) != '?' && i < 20) {
        i++;
      }
      strcpy(pshmem[i].destination, dest);
      pshmem[i].nbSeats = atoi(seats);
}

void main() {

  int pid;
  int cleMem = 200;
  int cleMutex = 100;
  int cleRemainingDest = 400;

  signal(SIGCONT, lecture);

  // Initialising and creating the mutex
  int idMutex  = create_semaphore(cleMutex);
  if(init_semaphore(idMutex, 1)==-1) exit(1);
  printf("Mutex initialisé\n");

  // Creating the semaphore counting the amount of destinations remaining
  int idRemainingDest = create_semaphore(cleMutex);
  if(init_semaphore(idRemainingDest, 20)==-1) exit(1);
  printf("Semaphore initialisé\n");

  // Creating the shared memory
  int idMem;
  if ((idMem = create_shmem(cleMem, sizeof(SHMEM)*22)) == -1){
    perror("create_shmem");
    exit(-1);
  }
  printf("%d\n", idMem);

  printf("Mem initialisé\n");
  SHMEM *ShrdMem = (SHMEM *)attach_shmem(idMem);
  initShareMem(ShrdMem);

  // Creating the pipe
  int desc[2];
  if( pipe(desc) == -1) { printf("erreur de création du pipe\n");exit(1);}
  printf("pipe initialisé\n");
  /*
   *
   * LeFILS : Tire des destinations dans la lsite et un nombre de places aléatoire et les envoie dans le tube pour le prc PERE
   *
   * */
  switch(pid = fork()) {

    case -1:
      fprintf(stderr, "Erreur lors de la création du fils.\n");
      break;
    case 0: // The son of the process : Tirage
	  close(desc[0]);
      
      int dest;
      int seats;
      const char* destination[] = {"Paris","New-York","Londres","Tokyo","Amsterdam","Lyon","Pyongyang","Rome","La-Havane","Sydney","San-Francisco","Seoul","Munich","Shangai","Hong-Kong","Singapour","Ho-Chi-Minh","Rio-de-Janeiro","Kuala-Lumpu","We-Are-Number-One"};
      int i = 0;

      while(1) {
		 //printf("entrée boucle fils\n");
         sleep(3);
         //printf("sortie sommeil fils\n");
         srand(time(NULL));
         seats = rand()%(0-10)+10;// Picks a random number between 0 and 100
         printf("val seats : %d\n", seats);
		     char toSend[TAILLEMSGTUBE] = "??????????????????????\0";

		     //printf("buffereee : %s\n", intBuffer);
		     int j;
		     for (j = 0; j < strlen(destination[i]); j++) {
			     toSend[j] = destination[i][j];
		     }


		 char intBuffer[2] = "";
		 sprintf(intBuffer, "%d", seats);
		 toSend[TAILLEMSGTUBE-2] = intBuffer[0];
     toSend[TAILLEMSGTUBE-1] = intBuffer[1];
		 printf("intbuff : %s \n", intBuffer);
		 printf("dans le tube : %s \n", toSend);
      /*Forme des envois dans le tube : 23 char : Destination + Bourrage (?) + Places */
		 write(desc[1], &toSend,23); //Ecriture de la nouvelle destination dans le pipe
		 kill(getppid(), SIGCONT); //Envoi du signal pour réveiller écrivain
		 i++;

		 if(i==20){i = 0;}	//10 = Taille de la liste de destinations
      }
      break;
    default: // The process itself Ecrivain
      /*
       *
       * Le PERE : Recois les infos du fils et les ecrit dans la mémoire
       * Fait aussi l'initialisation de la mémoire, sémaphore mutex, sémaphore 20 jetons
       *
       * */

       //printf("Entrée père\n");
	     close(desc[1]); //Ecrivain n'a pas besoin d'écrire dans le pipe
	     signal(SIGINT, finEcrivain);
	     //Ajout du PID Ecrivain 
	     down(idMutex);
	     ShrdMem[0].pid = getpid();
	     ShrdMem[1].pid = pid;     
         up(idMutex);
         
         /* Tableau des PIDs
          * 0 = Ecrivain
          * 1 = Tirage
          * 2 = Agence
          * 3 = Client
          * 4 = Affichage
         */	     

       while(1){
	    //printf("entree boucle pere\n");
        char toAdd[TAILLEMSGTUBE] = "";
        pause();
        //printf("sortie pause pere\n");
        down(idRemainingDest);
        printf("down(idRemainingDest)\n");
        read(desc[0],&toAdd,23);
        printf("dans le pere : %s \n", toAdd);
        
        //on ecrit gentillement la nouvelle entrée dans la case de Database qui va bien
        down(idMutex);
        addShareMem(ShrdMem, toAdd);
        afficheShareMem(ShrdMem);
        up(idMutex);
	    printf("PERE : UP MUTEX\n");

      }
      break;
  }

}
