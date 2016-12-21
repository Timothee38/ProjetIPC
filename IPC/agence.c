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

void fin(){
	int idBoite;
	if ((idBoite = msgget(300, 0666|IPC_CREAT))==-1){
		exit(-1);
	}
	msgctl(idBoite, IPC_RMID, NULL);
	exit(1);
}

void verificationMemoire(SHMEM* ShrdMem, char *commande, int seats){
	printf("Entree Verif");
	int i;
	int j;
	int ok = 0;

  for(i = 0; i < 20; i++) {
	  ok = 0;
	  for(j = 0; j < 20; j++){
		if(ShrdMem[i].destination[j] == commande[j]){
			ok = ok+1;
		 }
	  }
   }
   if(ok == 20){
		if(ShrdMem[i].nbSeats >= seats){
		ShrdMem[i].nbSeats = ShrdMem[i].nbSeats - seats;
		printf("Commande Valide!\n");
		}
	}else{
		printf("Rien de valide!\n");
	}
}


void commandeValide(){
	//Ne fait rien pour le moment
}

void commandeInvalide(){
	//Ne fait rien pour le moment
}

void main(){

	struct msgbuf{
		long messageType;
		char mess[20]; //20 ou 21?
	} message, commande;

	int cleMutex = 100;
	int cleShMem = 200;
	key_t cleBoite = 300;

	int pid;
	int idBoite;
	int idMutex;

	idMutex = open_semaphore(cleMutex);
	printf("Semaphore Mutex Ouvert\n");


	pid = getpid();

	signal(SIGKILL, fin); //Protection pour la fermeture

	//Création de la boite aux lettres
	if((idBoite = msgget(cleBoite, 0666|IPC_CREAT))==-1){
		perror("Erreur boite aux lettres");
		exit(-1);
	}
	printf("Boite aux lettres crée!\n");


	//Création du fils
	switch(pid = fork()) {

    case -1:
      fprintf(stderr, "Erreur lors de la création du client.\n");
      break;
    case 0: // The son of the process : Client


		signal(SIGUSR1, commandeValide); //Protection des signaux de validation de l'agence
		signal(SIGUSR2, commandeInvalide);
		char comm = "";
		while(1){

			scanf("%s", &comm);

			commande.messageType = 1;
			sprintf(commande.mess,"%s",&comm);
			msgsnd(idBoite, &commande, sizeof(commande.mess),0);
			printf("Message Envoyé!\n");
			}

	default : //Le process Agence lui même
		printf("Bienvenue chez l'agence de Voyage, les derniers vols sont actualisés régulièrement!\n");
		int i;
		int seats = 0;
		SHMEM *ShrdMem = (SHMEM *)attach_shmem(open_shmem(cleShMem,sizeof(SHMEM)*20));
		char destRecue[21];
		int selection = 0;

		printf("Mémoire connectée!\n");
		while(selection == 0){

			destRecue[21] = "????????????????????\0";




			msgrcv(idBoite, &message, cleBoite, 1,0);
			printf("Message recu\n");
			sleep(1);
			printf("Commande Recue : %s\n", message.mess);
			printf("longeur chaine recue : %d\n", strlen(message.mess));

			if(strlen(message.mess) > 2){
			for(i=0; i < strlen(message.mess); i++){
				destRecue[i] = message.mess[i];
			}
			printf("Destination Recue : %s\n", destRecue);
			}else{
				seats = atoi(message.mess);
				printf("Commande Recue : %d\n", seats);
				selection = 1;

			}

			//Verifications a faire dans la mémoire partagée et renvoyer le signal correspondant

		}
		printf("Entree mutex\n");
			down(idMutex);

			verificationMemoire(ShrdMem, destRecue,seats);
			up(idMutex);
	}

}
