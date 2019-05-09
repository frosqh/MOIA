#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include "fonctionsTCP.h"
#include "protocolYokai.h"


//une fonction qui fait la réception d'un coup du moteur IA (Java) 
TCoupReq construireCoup(int socket,TSensTetePiece sens,int partie,bool* breaker ){
	int err;
	TCoupReq coupReq;
	Coup coupJava;

	err = recv(socket, &coupJava, sizeof(Coup), MSG_PEEK);
    while(err < sizeof(Coup)){
       if (err <= 0) {
         perror("(joueur) erreur dans la reception du coup Java");
         shutdown(socket, SHUT_RDWR); close(socket);
        *breaker = true;
    	}
       err = recv(socket, &coupJava, sizeof(Coup), MSG_PEEK);
    }

	err = recv(socket, &coupJava, sizeof(Coup), 0);
	if (err <= 0) {
		perror("(joueur) erreur dans la reception du coup Java");
		shutdown(socket, SHUT_RDWR); close(socket);
		*breaker = true;
	}
	//Partie commune pour tous les coups
	coupReq.idRequest = ntohl(coupJava.idReq);
	coupReq.numPartie = ntohl(coupJava.numPartie);
	coupReq.typeCoup = ntohl(coupJava.typeCoup);
	coupReq.piece.sensTetePiece = ntohl(coupJava.sensPiece);
	coupReq.piece.typePiece = ntohl(coupJava.typePiece);

	//on teste le type du coup recu de Java
	switch(coupJava.typeCoup){

		//cas d'un coup DEPLACER
		case 0 :
			coupReq.params.deplPiece.caseDep.c = ntohl(coupJava.colonneDep);
			coupReq.params.deplPiece.caseDep.l = ntohl(coupJava.ligneDep);
			coupReq.params.deplPiece.caseArr.c = ntohl(coupJava.colonneArr);
			coupReq.params.deplPiece.caseArr.l = ntohl(coupJava.ligneArr);
			printf("Coup calculé par l'IA : DEPLACER\n");
		break;

		//cas d'un coup DEPOSER
		case 1 :
			coupReq.params.deposerPiece.c = ntohl(coupJava.colonneArr);
			coupReq.params.deposerPiece.l = ntohl(coupJava.ligneArr);
			printf("Coup calculé par l'IA : DEPOSER\n");
		break;

		default : 
			*breaker = true;
			printf("Problème avec le coup recu de l'IA, type de coup inconnu ! Partie quittée\n");

		break;
	}
	
	if(ntohl(coupJava.capture)==0) coupReq.params.deplPiece.estCapt = false;
	else coupReq.params.deplPiece.estCapt = true;

	printf("construction coup finie \n");
	return coupReq;
}


//Fait le traitement de la saisie du nom/sens et rends le sens accordé par le serveur

TSensTetePiece debutPartie(int sock, TPartieReq partieReq, TPartieRep partieRep){

	TSensTetePiece sens, sensAccorde;
	char sensDemande;
	int err;

	partieReq.idReq = PARTIE;
    printf("Veuillez indiquer le nom du joueur : \n");
    fgets(partieReq.nomJoueur,T_NOM,stdin);
    printf("Veuillez indiquer le sens souhaité : \n");
    scanf("%c", &sensDemande);
    
    
    if (sensDemande == 's')
    {
    	partieReq.piece = SUD;
    	printf("Sens souhaité: SUD \n");
    	sens = SUD;
    	
    }
    if (sensDemande == 'n')
    {
    	partieReq.piece = NORD;
    	printf("Sens souhaité: NORD\n");
    	sens = NORD;
    }
    
    
    err = send(sock, &partieReq, sizeof(TPartieReq), 0);
    if (err <= 0){
       perror("(client) erreur sur le send de requete de demander de partie");
       shutdown(sock, SHUT_RDWR); close(sock);
       return -3;
    }
    
    //Reception de la reponse Partie

	err = recv(sock, &partieRep, sizeof(TPartieRep), 0);
	if (err <= 0) {
		perror("(joueur) erreur dans la reception de validation de demande de partie.");
		shutdown(sock, SHUT_RDWR); close(sock);
		return -4;
	}

	//Le code de la reponse recue
	switch(partieRep.err){
		case ERR_OK :
			printf("Validation de la requete de demande de partie.\n");

		break;
		case ERR_PARTIE :
			printf("Erreur sur la  requete de demande de partie.\n");
			return -5;
		break;
		default :
			printf("Erreur sur la requete de demande de partie (mauvaise saisie).\n");
			return -6;
		break;
	}

	//Extraire les informations de la reponse recue

	if (partieRep.validSensTete == OK)
	{
		
		if (sens == NORD)
		{
			sensAccorde = NORD;
			printf("Sens accordé: NORD (correspend au sens demandé).\n");
		}
		else{
			sensAccorde = SUD;
			printf("Sens accordé: SUD (correspend au sens demandé).\n");
		}
		
	}
	if (partieRep.validSensTete == KO){
		if (sens == NORD)
		{
			sensAccorde = SUD;
			printf("Sens accordé: SUD (l'inverse du sens demandé)\n");
		}
		else{
			sensAccorde = NORD;
			printf("Sens accordé: NORD (l'inverse du sens demandé)\n");
		}
		
	}

	return sensAccorde;
}
