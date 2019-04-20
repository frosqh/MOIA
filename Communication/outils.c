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


TCoupReq construireCoup(int socket,TSensTetePiece sens,int partie){
	int err;
	TCoupReq coupReq;
	Coup coupJava;
	err = recv(socket, &coupJava, sizeof(Coup), MSG_PEEK);

    while(err < sizeof(Coup)){
       if (err <= 0) {
         perror("(joueur) erreur dans la reception du coup Java");
         shutdown(socket, SHUT_RDWR); close(socket);
         exit(-10);
    	}
       err = recv(socket, &coupJava, sizeof(Coup), MSG_PEEK);
    }

	err = recv(socket, &coupJava, sizeof(Coup), 0);
	if (err <= 0) {
		perror("(joueur) erreur dans la reception du coup Java");
		shutdown(socket, SHUT_RDWR); close(socket);
		exit(-11);
	}
	coupReq.idRequest = COUP;
	coupReq.numPartie = partie;
	coupReq.typeCoup = DEPLACER;
	coupReq.piece.sensTetePiece = sens;
	coupReq.piece.typePiece = ntohl(coupJava.piece);
	coupReq.params.deplPiece.caseDep.c = ntohl(coupJava.colonneDep);
	coupReq.params.deplPiece.caseDep.l = ntohl(coupJava.ligneDep);
	coupReq.params.deplPiece.caseArr.c = ntohl(coupJava.colonneArr);
	coupReq.params.deplPiece.caseArr.l = ntohl(coupJava.ligneArr);
	coupReq.params.deplPiece.estCapt = ntohl(coupJava.capture);

	printf("construction coup finie \n");
	return coupReq;
}

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
