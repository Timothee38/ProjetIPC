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

void finAgence(){

	//Fermeture de la boite
	int idBoite;
	if ((idBoite = msgget(300, 0666|IPC_CREAT))==-1){
		perror("Erreur boite aux lettres");
		exit(-1);
	}
	msgctl(idBoite, IPC_RMID, NULL);

	int idMutex = open_semaphore(100);
	SHMEM *ShrdMem = (SHMEM *)attach_shmem(open_shmem(200,sizeof(SHMEM)*22));
	printf("Fermeture Agence\n");
	kill(ShrdMem[0].pid, SIGINT);  //Fermeture finale de l'écrivain une 2eme fois pour être sur
	exit(1);

}

 int verificationMemoire(SHMEM* ShrdMem, char *commande, int seats, int longDest, int pidClient){
 	int i;
 	int j;
 	int ok = 0;
 	int retour = -1;
 	
   for(i = 0; i < 20; i++) {
 	  ok = 0;
 	  
 	  for(j = 0; j < 20; j++){
 		  //printf("SHRDMEM =  %c |Commande = %c \n", ShrdMem[i].destination[j],commande[j]); //Debug
 		  if(ShrdMem[i].destination[j] == '?'){
 		     j = 21;
 		  }else{
 			 if(ShrdMem[i].destination[j] == commande[j]){
 				ok = ok+1;
 				//printf("ok = %d\n", ok);
 				if(ok == longDest){
 					if(ShrdMem[i].nbSeats >= seats){
 						//printf("SHRDMEM sts =  %d |sts = %d \n", ShrdMem[i].nbSeats,seats);
 						ShrdMem[i].nbSeats = ShrdMem[i].nbSeats - seats;
-						printf("Commande Valide!\n");
+						//printf("Commande Valide!\n");
 						kill(pidClient, SIGUSR1);
 						j = 21;
 						i = 21;
 						retour = 0;
 				  }
 			   }
 			}
 		 }
 	  }
    }
    if(retour != 0){kill(pidClient, SIGUSR2);}
    return retour;
}

void main(){

	//Déclarations des variables
	key_t cleBoite = 300;
	int pid;
	int idBoite;
	int cleMutex = 100;
	int cleShMem = 200;
	int idMutex;

	//Protection pour la fermeture
	signal(SIGINT, finAgence);

	//Connexion au mutex et à la mémoire des destinations
	idMutex = open_semaphore(cleMutex);
	//printf("Semaphore Mutex Ouvert\n");

	down(idMutex);
	SHMEM *ShrdMem = (SHMEM *)attach_shmem(open_shmem(cleShMem,sizeof(SHMEM)*22));
	up(idMutex);
	//printf("Mémoire connectée!\n");

	//Ajout du PID Agence
	down(idMutex);
	ShrdMem[2].pid = getpid();
	ShrdMem[3].pid = pid;
	up(idMutex);

	//Création de la boite aux lettres
	if((idBoite = msgget(cleBoite, 0666|IPC_CREAT))==-1){
		perror("Erreur Creation boite aux lettres");
		exit(-1);
	}
	//printf("Boite aux lettres crée!\n");

	printf("Bienvenue chez l'agence de Voyage, les derniers vols sont actualisés régulièrement!\n");

	// Reception MSGs
int i;

	while(1) {
		MSG received;
		char dest[21]; char seats[3]; char tempPidBuffer[10]; int clientPid;

		msgrcv(idBoite, &received, sizeof(received.mtext), 1, 0);
		printf("message reçu; %s ", received.mtext);

		sscanf(received.mtext, "%s %s %s", dest, seats, tempPidBuffer);

		strcat(dest, "\0");
		strcat(seats, "\0");

		clientPid = atoi(tempPidBuffer);

		printf("Destination: %s, Seats: %s, pid: %d \n", dest, seats, clientPid);

		// Et maintenant, traitement
		down(idMutex);
		int vr = verificationMemoire(ShrdMem, dest, atoi(seats), strlen(dest), clientPid);
		if (vr == -1) {
			printf("Agence: Commande échouée\n");
		}
		else if (vr == 0) {
			printf("Agence: Commande réussie\n");
		}
		up(idMutex);

		//kill(clientPid, SIGKILL);

	}


/*		//Vérification et écritoure dans la mémoire
		down(idMutex);
		int vr = verificationMemoire(ShrdMem, comma.mess,atoi(seats.mess),strlen(comma.mess), pid);
		if(vr == -1){printf("Agence : commande echouée\n");}
		if(vr == 0){printf("Agence : commande réussie\n");}
		up(idMutex);

		//Fermeture des processus
		kill(pid, SIGKILL);
*/

}
