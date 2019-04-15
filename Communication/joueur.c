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

//// 12/04/2019 
//// version beta joueur
//// testé : envoi et reception de réponse de types coup et partie (OK)
//// à faire après (par moi biensur) : tout mettre dans des fonctions




int main(int argc, char **argv){
	int sock,port,err;                 
	char* nomMachServ;       
	TPartieReq partieReq;
	TPartieRep partieRep;
	TCoupReq coupReq;
	TCoupReq coupReqAdversaire;
	char sensDemande;
	TSensTetePiece sensAccorde;
	TSensTetePiece sens;
	bool sensValide = false;
	bool finished = false;
	TCodeRep codeRepCoup;
	int boucle = 0;	
	TLg coup[6]= {UN,DEUX,TROIS,QUATRE,CINQ,SIX};
	
	
	 
	/* verification des arguments */
	if (argc != 3) {
	   printf("usage : %s nom/IPServ port\n", argv[0]);
	    return -1;
	}
	  
	nomMachServ = argv[1];
	port = atoi(argv[2]);

	sock = socketClient(nomMachServ, port);
	if (sock < 0) {
	    perror("(client) erreur sur la creation de socket");
	    return -2;
	}

	//Envoi de la requete Partie

	partieReq.idReq = PARTIE;
    printf("Veuillez indiquer le nom du joueur : \n");
    fgets(partieReq.nomJoueur,T_NOM,stdin);

    printf("Veuillez indiquer le sens souhaité : \n");
    scanf("%c", &sensDemande);
    
    if (sensDemande == 's')
    {
    	partieReq.piece = SUD;
    	printf("Sens souhaité sud : \n");
    	sens = SUD;
    	
    }
    if (sensDemande == 'n')
    {
    	partieReq.piece = NORD;
    	printf("Sens souhaité nord : \n");
    	sens = NORD;
    }
    
    
    err = send(sock, &partieReq, sizeof(TPartieReq), 0);
    if (err <= 0){
      perror("(client) erreur sur le send");
      shutdown(sock, SHUT_RDWR); close(sock);
      return -3;
    }
    
    //Reception de la reponse Partie

	err = recv(sock, &partieRep, sizeof(TPartieRep), 0);
	if (err <= 0) {
		perror("(clientTCP) erreur dans la reception");
		shutdown(sock, SHUT_RDWR); close(sock);
		return -4;
	}

	//Le code de la reponse recue
	switch(partieRep.err){
		case ERR_OK :
			printf("Validation de la demande de partie : \n");

		break;
		case ERR_PARTIE :
			printf("Erreur sur la demande de partie : \n");
			return -5;
		break;
		default :
			printf("Erreur sur la requete : \n");
			return -6;
		break;
	}

	//Extraire les informations de la reponse recue

	if (partieRep.validSensTete == OK)
	{
		if (sens == NORD)
		{
			sensAccorde = NORD;
			printf("le sens accordé est : nord\n");
		}
		else{
			sensAccorde = SUD;
			printf("le sens accordé est : sud\n");
		}
		
	}
	if (partieRep.validSensTete == KO){
		if (sens == NORD)
		{
			sensAccorde = SUD;
			printf("le sens accordé est : nord\n");
		}
		else{
			sensAccorde = NORD;
			printf("le sens accordé est : sud\n");
		}
		
	}

	//Le sens voulu est accorde ou pas

	if (sensAccorde == sens)
	{
		sensValide = true;
		printf("le sens accordé correspend au sens demandé\n");

	}
	else{
		printf("le sens accordé est le sens inverse\n");
	}

	switch (sensAccorde){
		//on commence en premier
		printf("on est le joueur sud on joue en premier\n");
		case SUD:
			//////**************************************
			while(boucle < 4){
				//Construction d'une requete provisoire :
				coupReq.idRequest = COUP;
				coupReq.numPartie = 1;
				coupReq.typeCoup = DEPLACER;
				coupReq.piece.typePiece = ONI;
				coupReq.piece.sensTetePiece = SUD;
				coupReq.params.deplPiece.caseDep.c = E;
				coupReq.params.deplPiece.caseDep.l = coup[boucle];
				coupReq.params.deplPiece.caseArr.c = E;
				coupReq.params.deplPiece.caseArr.l = coup[boucle+1];
				coupReq.params.deplPiece.estCapt = false;

				

				err = send(sock, &coupReq, sizeof(TCoupReq), 0);
    			if (err <= 0){
      				perror("(joueur) erreur sur le send coup");
      				shutdown(sock, SHUT_RDWR); close(sock);
      			return -7;
    			}

    			//Reception de la reponse validation coup joué

				err = recv(sock, &codeRepCoup, sizeof(TCodeRep), MSG_PEEK);
				if (err <= 0) {
					perror("(joueur) erreur dans la reception de la validation coup");
					shutdown(sock, SHUT_RDWR); close(sock);
					return -4;
				}

				if (codeRepCoup == ERR_OK)
				{
					printf("1 valide !\n");
				}
				else{
					printf("1 non valide !\n");
				}

				//Reception de la reponse validation coup joué par adversaire
				printf("attente coup adversaire avant le recv!\n");

				err = recv(sock, &codeRepCoup, sizeof(TCodeRep), 0);
				if (err <= 0) {
					perror("(clientTCP) erreur dans la reception");
					shutdown(sock, SHUT_RDWR); close(sock);
					return -4;
				}

				if (codeRepCoup == ERR_OK)
				{
					printf("2 coup adversaire valide  !\n");
				}
				else{
					printf("2 coup adversaire non valide !\n");
				}


				//Reception du coup de l'adversaire

				//Reception de la reponse validation coup joué par adversaire

				err = recv(sock, &coupReqAdversaire, sizeof(TCoupReq), 0);
				if (err <= 0) {
					perror("(clientTCP) 3 erreur dans la reception");
					shutdown(sock, SHUT_RDWR); close(sock);
					return -4;
				}
				else{
					printf("3 recv valide  !\n");
				}


				

				



				boucle++;
			}

		break;

		case NORD:
			printf("on est le joueur nord on joue en deuxieme \n");
			//////**************************************
			while(boucle<3){
				//Reception de la reponse validation coup (adversaire)
				
				
				//Reception de la reponse validation coup joué par adversaire

				err = recv(sock, &codeRepCoup, sizeof(TCodeRep), 0);
				if (err <= 0) {
					perror("(clientTCP) erreur dans la reception");
					shutdown(sock, SHUT_RDWR); close(sock);
					return -4;
				}

				if (codeRepCoup == ERR_OK)
				{
					printf("1 coup adversaire valide  !\n");
				}
				else{
					printf("1 coup adversaire non valide !\n");
				}


				//Reception du coup de l'adversaire

				//Reception de la reponse validation coup joué par adversaire

				err = recv(sock, &coupReqAdversaire, sizeof(TCoupReq), 0);
				if (err <= 0) {
					perror("(clientTCP) erreur dans la reception");
					shutdown(sock, SHUT_RDWR); close(sock);
					return -4;
				}
				else{
					printf("2 coup adversaire valide  !\n");
				}


				coupReq.idRequest = COUP;
				coupReq.numPartie = 1;
				coupReq.typeCoup = DEPLACER;
				coupReq.piece.typePiece = ONI;
				coupReq.piece.sensTetePiece = NORD ;
				coupReq.params.deplPiece.caseDep.c = A;
				coupReq.params.deplPiece.caseDep.l = coup[5-boucle];
				coupReq.params.deplPiece.caseArr.c = A;
				coupReq.params.deplPiece.caseArr.l = coup[5-(boucle+1)];
				coupReq.params.deplPiece.estCapt = false;


				err = send(sock, &coupReq, sizeof(TCoupReq), 0);
    			if (err <= 0){
      				perror("(client) erreur sur le send");
      				shutdown(sock, SHUT_RDWR); close(sock);
      			return -7;
    			}

    			//Reception de la reponse validation coup joué

				err = recv(sock, &codeRepCoup, sizeof(TCodeRep), MSG_PEEK);
				if (err <= 0) {
					perror("(clientTCP) erreur dans la reception");
					shutdown(sock, SHUT_RDWR); close(sock);
					return -4;
				}

				if (codeRepCoup == ERR_OK)
				{
					printf("3 coup valide mabrouk !\n");
				}
				else{
					printf("3 coup non valide !\n");
				}


    			


				boucle++;
			}

		break;
	}




	//Fermeture de la socket joueur
	shutdown(sock, SHUT_RDWR);
	close(sock);



    



    
    
    
  

  return 0;
}



  

  
