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

void commandeValide(){
	//Ne fait rien pour le moment
}

void commandeInvalide(){
	//Ne fait rien pour le moment
}

void main(){
	int pid;
	key_t cleBoite = 300;
	int idBoite;
	
	struct msgbuf{
		long messageType;
		char mess[20]; //20 ou 21?
	} message, commande;
	
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
		char comm[20] = "";
		while(1){
			
			scanf("%s", &comm);
			
			commande.messageType = 1;
			sprintf(commande.mess,"%s",comm);
			msgsnd(idBoite, &commande, sizeof(commande.mess),0);
			printf("Message Envoyé!\n");
			}
	
	default : //Le process Agence lui même
		printf("Bienvenue chez l'agence de Voyage, les derniers vols sont actualisés régulièrement!\n");
		while(1){
			
			msgrcv(idBoite, &message, cleBoite, 1,0);
			printf("Message recu\n");
			sleep(1);
			printf("Commande Recue : %s\n", message.mess);
			
			//Verifications a faire dans la mémoire partagée et renvoyer le signal correspondant
		
		}
	}
	
}

