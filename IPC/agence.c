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
	kill(ShrdMem[3].pid, SIGKILL); //Femrmeture du client
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
						//printf("Commande Valide!\n");
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

	//Structure d'un message
	struct msgbuf{
		long messageType;
		char mess[20];
	} comma, commande, seats;

	//Déclarations des variables
	key_t cleBoite = 300;
	int pid;
	int idBoite;

	//Protection pour la fermeture


	//Création de la boite aux lettres
	if((idBoite = msgget(cleBoite, 0666|IPC_CREAT))==-1){
		perror("Erreur boite aux lettres");
		exit(-1);
	}
	//printf("Boite aux lettres crée!\n");

	printf("Bienvenue chez l'agence de Voyage, les derniers vols sont actualisés régulièrement!\n");

	//Création du Client
	switch(pid = fork()) { //remove this

	//Cas d'erreur
    case -1:

      fprintf(stderr, "Erreur lors de la création du client.\n");
      break;

    //the son of the process : Client
    case 0:

		//Protection des signaux de validation de l'agence
		signal(SIGUSR1, commandeValide);
		signal(SIGUSR2, commandeInvalide);

		char sieges[2] = "";
		int i;

		//Saisie de la commande
		printf("Entrez votre destination :\n");
		scanf("%s", &commande.mess);
		printf("Combien souhaitez vous de places pour %s :\n", commande.mess);
		scanf("%s", &sieges);

		//Envoi de la commande en deux parties
		commande.messageType = 1;
		msgsnd(idBoite, &commande, sizeof(commande.mess),0);

		for(i = 0; i< strlen(sieges); i++){
			commande.mess[i] = sieges[i];
		}
		commande.messageType = 2;
		msgsnd(idBoite, &commande, 8,0);

	//Le process Agence lui même
	default :

		signal(SIGINT, finAgence);
		int cleMutex = 100;
	    int cleShMem = 200;

		int idMutex;

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

        //Reception de la destination puis du nombre de sièges
		msgrcv(idBoite, &comma, cleBoite, 1,0);
		msgrcv(idBoite, &seats, cleBoite, 2,0);

		/* Debug
		printf("Message recu\n");
		printf("Commande Recue : %s\n", comma.mess);
		printf("longeur chaine recue : %d\n", strlen(comma.mess));
		printf("Sièges Recus : %d\n", atoi(seats.mess));
		*/

		//Vérification et écritoure dans la mémoire
		down(idMutex);
		int vr = verificationMemoire(ShrdMem, comma.mess,atoi(seats.mess),strlen(comma.mess), pid);
		if(vr == -1){printf("Agence : commande echouée\n");}
		if(vr == 0){printf("Agence : commande réussie\n");}
		up(idMutex);

		//Fermeture des processus
		kill(pid, SIGKILL);



	}


}
