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
	char colonneProv,colonneProvArr,ligneProv,ligneProvArr,pieceProv;
	TSensTetePiece sensAccorde;
	TSensTetePiece sens;
	TCoupRep coupRep,coupRepAdv;
		
	
	
	
	 
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
			printf("Sens accordé: NORD (correspend au sens demandé)\n");
		}
		else{
			sensAccorde = SUD;
			printf("Sens accordé: SUD (correspend au sens demandé)\n");
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
	//*****************************
	//******* Partie **************
	//***************************** 

	switch (sensAccorde){
		//on commence en premier
		
		case SUD:
			while(getchar() != '\n');
			printf("C'est à vous de commencer \n");
			//////**************************************
			while(true){
				//Construction d'une requete provisoire :
				//*********************************************
				printf("Choisir la piece : \n");
    			scanf("%c", &pieceProv);

    			printf("piece choisie :%c\n", pieceProv);
    			while(getchar() != '\n');
    			switch (pieceProv){
    				case 'k':
    					coupReq.piece.typePiece = KODAMA;
    				break;
    				case 'r':
    					coupReq.piece.typePiece = KODAMA_SAMOURAI;
    				break;
    				case 'i':
    					coupReq.piece.typePiece = KIRIN;
    				break;
    				case 'p':
    					coupReq.piece.typePiece = KOROPOKKURU;
    				break;
    				case 'o':
    					coupReq.piece.typePiece = ONI;
    				break;
    				case 's':
    					coupReq.piece.typePiece = SUPER_ONI;
    				break;


    			}
    			
    			printf("Choisir la colonne de depart (A,B,C,D,E) : \n");
    			scanf("%c", &colonneProv);
    			while(getchar() != '\n');
    			switch (colonneProv){
    				case 'a':
    					coupReq.params.deplPiece.caseDep.c = A;
    				break;
    				case 'b':
    					coupReq.params.deplPiece.caseDep.c = B;
    				break;
    				case 'c':
    					coupReq.params.deplPiece.caseDep.c = C;
    				break;
    				case 'd':
    					coupReq.params.deplPiece.caseDep.c = D;
    				break;
    				case 'e':
    					coupReq.params.deplPiece.caseDep.c = E;
    				break;


    			}
    			
    			printf("Choisir la ligne de depart (UN,DEUX....) : \n");
    			scanf("%c", &ligneProv);
    			while(getchar() != '\n');
    			switch (ligneProv){
    				case '1':
    					coupReq.params.deplPiece.caseDep.l = UN;
    				break;
    				case '2':
    					coupReq.params.deplPiece.caseDep.l = DEUX;
    				break;
    				case '3':
    					coupReq.params.deplPiece.caseDep.l = TROIS;
    				break;
    				case '4':
    					coupReq.params.deplPiece.caseDep.l = QUATRE;
    				break;
    				case '5':
    					coupReq.params.deplPiece.caseDep.l = CINQ;
    				break;
    				case '6':
    					coupReq.params.deplPiece.caseDep.l = SIX;
    				break;


    			}
    			
    			printf("Choisir la colonne d'arrivée (A,B,C,D,E) : \n");
    			scanf("%c", &colonneProvArr);
    			while(getchar() != '\n');
    			switch (colonneProvArr){
    				case 'a':
    					coupReq.params.deplPiece.caseArr.c = A;
    				break;
    				case 'b':
    					coupReq.params.deplPiece.caseArr.c = B;
    				break;
    				case 'c':
    					coupReq.params.deplPiece.caseArr.c = C;
    				break;
    				case 'd':
    					coupReq.params.deplPiece.caseArr.c = D;
    				break;
    				case 'e':
    					coupReq.params.deplPiece.caseArr.c = E;
    				break;


    			}
    			
    			printf("Choisir la ligne d'arrivée (UN,DEUX....) : \n");
    			scanf("%c", &ligneProvArr);
    			while(getchar() != '\n');
    			switch (ligneProvArr){
    				case '1':
    					coupReq.params.deplPiece.caseArr.l = UN;
    				break;
    				case '2':
    					coupReq.params.deplPiece.caseArr.l = DEUX;
    				break;
    				case '3':
    					coupReq.params.deplPiece.caseArr.l = TROIS;
    				break;
    				case '4':
    					coupReq.params.deplPiece.caseArr.l = QUATRE;
    				break;
    				case '5':
    					coupReq.params.deplPiece.caseArr.l = CINQ;
    				break;
    				case '6':
    					coupReq.params.deplPiece.caseArr.l = SIX;
    				break;


    			}
    			


				coupReq.idRequest = COUP;
				coupReq.numPartie = 1;
				coupReq.typeCoup = DEPLACER;
				coupReq.piece.sensTetePiece = SUD;
				coupReq.params.deplPiece.estCapt = false;

				
				//*******************************************************************

				err = send(sock, &coupReq, sizeof(TCoupReq), 0);
    			if (err <= 0){
      				perror("(joueur) erreur sur le send coup");
      				shutdown(sock, SHUT_RDWR); close(sock);
      			return -7;
    			}

    			//Reception de la reponse validation coup joué

				err = recv(sock, &coupRep, sizeof(TCoupRep), 0);
				if (err <= 0) {
					perror("(joueur) erreur dans la reception de la validation coup");
					shutdown(sock, SHUT_RDWR); close(sock);
					return -4;
				}

				if (coupRep.err == ERR_OK && coupRep.validCoup != TRICHE && coupRep.propCoup == CONT)
				{
					printf("Coup valide !\n");
				}
				else{
					printf("Coup non valide partie finie !\n");
					break;
				}

				//Reception de la reponse validation coup joué par adversaire
				printf("attente coup adversaire avant le recv!\n");

				err = recv(sock, &coupRepAdv, sizeof(TCoupRep), 0);
				if (err <= 0) {
					perror("(clientTCP) erreur dans la reception");
					shutdown(sock, SHUT_RDWR); close(sock);
					return -4;
				}

				if (coupRepAdv.err == ERR_OK && coupRepAdv.validCoup != TRICHE && coupRepAdv.propCoup == CONT)
				{
					printf("Coup valide !\n");
				}
				else{
					printf("Coup non valide partie finie !\n");
					break;
				}


				//Reception du coup de l'adversaire

				//Reception de la reponse coup joué par adversaire

				err = recv(sock, &coupReqAdversaire, sizeof(TCoupReq), 0);
				if (err <= 0) {
					perror("(clientTCP) 3 erreur dans la reception");
					shutdown(sock, SHUT_RDWR); close(sock);
					return -4;
				}
				
				
			}

		break;

		case NORD:
			printf("Vous jouez en deuxième\n");
			//////**************************************
			while(true){
				//Reception de la reponse validation coup (adversaire)
				
				
				//Reception de la reponse validation coup joué par adversaire

				//Reception de la reponse validation coup joué par adversaire
				

				err = recv(sock, &coupRepAdv, sizeof(TCoupRep), 0);
				if (err <= 0) {
					perror("(clientTCP) erreur dans la reception");
					shutdown(sock, SHUT_RDWR); close(sock);
					return -4;
				}

				if (coupRepAdv.err == ERR_OK && coupRepAdv.validCoup != TRICHE && coupRepAdv.propCoup == CONT)
				{
					printf("Coup adversaire valide !\n");
				}
				else{
					printf("Coup adversaire non valide partie finie !\n");
					break;
				}


				//Reception du coup de l'adversaire

				//Reception de la reponse coup joué par adversaire

				err = recv(sock, &coupReqAdversaire, sizeof(TCoupReq), 0);
				if (err <= 0) {
					perror("(clientTCP) erreur dans la reception");
					shutdown(sock, SHUT_RDWR); close(sock);
					return -4;
				}


				//Construction d'une requete provisoire :
				//*********************************************
				while(getchar() != '\n');
				printf("Choisir la piece : \n");
    			scanf("%c", &pieceProv);
    			while(getchar() != '\n');
    			switch (pieceProv){
    				case 'k':
    					coupReq.piece.typePiece = KODAMA;
    				break;
    				case 'u':
    					coupReq.piece.typePiece = KODAMA_SAMOURAI;
    				break;
    				case 'i':
    					coupReq.piece.typePiece = KIRIN;
    				break;
    				case 'p':
    					coupReq.piece.typePiece = KOROPOKKURU;
    				break;
    				case 'o':
    					coupReq.piece.typePiece = ONI;
    				break;
    				case 's':
    					coupReq.piece.typePiece = SUPER_ONI;
    				break;


    			}
    			
    			
    			printf("Choisir la colonne de depart (A,B,C,D,E) : \n");
    			scanf("%c", &colonneProv);
    			while(getchar() != '\n');
    			switch (colonneProv){
    				case 'a':
    					coupReq.params.deplPiece.caseDep.c = A;
    				break;
    				case 'b':
    					coupReq.params.deplPiece.caseDep.c = B;
    				break;
    				case 'c':
    					coupReq.params.deplPiece.caseDep.c = C;
    				break;
    				case 'd':
    					coupReq.params.deplPiece.caseDep.c = D;
    				break;
    				case 'e':
    					coupReq.params.deplPiece.caseDep.c = E;
    				break;


    			}
    			
    			printf("Choisir la ligne de depart (UN,DEUX....) : \n");
    			scanf("%c", &ligneProv);
    			while(getchar() != '\n');
    			switch (ligneProv){
    				case '1':
    					coupReq.params.deplPiece.caseDep.l = UN;
    				break;
    				case '2':
    					coupReq.params.deplPiece.caseDep.l = DEUX;
    				break;
    				case '3':
    					coupReq.params.deplPiece.caseDep.l = TROIS;
    				break;
    				case '4':
    					coupReq.params.deplPiece.caseDep.l = QUATRE;
    				break;
    				case '5':
    					coupReq.params.deplPiece.caseDep.l = CINQ;
    				break;
    				case '6':
    					coupReq.params.deplPiece.caseDep.l = SIX;
    				break;


    			}
    			
    			printf("Choisir la colonne d'arrivée (A,B,C,D,E) : \n");
    			scanf("%c", &colonneProvArr);
    			while(getchar() != '\n');
    			switch (colonneProvArr){
    				case 'a':
    					coupReq.params.deplPiece.caseArr.c = A;
    				break;
    				case 'b':
    					coupReq.params.deplPiece.caseArr.c = B;
    				break;
    				case 'c':
    					coupReq.params.deplPiece.caseArr.c = C;
    				break;
    				case 'd':
    					coupReq.params.deplPiece.caseArr.c = D;
    				break;
    				case 'e':
    					coupReq.params.deplPiece.caseArr.c = E;
    				break;


    			}
    			
    			printf("Choisir la ligne d'arrivée (UN,DEUX....) : \n");
    			scanf("%c", &ligneProvArr);
    			while(getchar() != '\n');
    			switch (ligneProvArr){
    				case '1':
    					coupReq.params.deplPiece.caseArr.l = UN;
    				break;
    				case '2':
    					coupReq.params.deplPiece.caseArr.l = DEUX;
    				break;
    				case '3':
    					coupReq.params.deplPiece.caseArr.l = TROIS;
    				break;
    				case '4':
    					coupReq.params.deplPiece.caseArr.l = QUATRE;
    				break;
    				case '5':
    					coupReq.params.deplPiece.caseArr.l = CINQ;
    				break;
    				case '6':
    					coupReq.params.deplPiece.caseArr.l = SIX;
    				break;


    			}
    			



				coupReq.idRequest = COUP;
				coupReq.numPartie = 1;
				coupReq.typeCoup = DEPLACER;
				coupReq.piece.sensTetePiece = NORD;
				coupReq.params.deplPiece.estCapt = false;

				err = send(sock, &coupReq, sizeof(TCoupReq), 0);
    			if (err <= 0){
      				perror("(client) erreur sur le send");
      				shutdown(sock, SHUT_RDWR); close(sock);
      			return -7;
    			}

    			//Reception de la reponse validation coup joué

				err = recv(sock, &coupRep, sizeof(TCoupRep), 0);
				if (err <= 0) {
					perror("(joueur) erreur dans la reception de la validation coup");
					shutdown(sock, SHUT_RDWR); close(sock);
					return -4;
				}

				if (coupRep.err == ERR_OK && coupRep.validCoup != TRICHE && coupRep.propCoup == CONT)
				{
					printf("Coup valide !\n");
				}
				else{
					printf("Coup non valide partie finie !\n");
					break;
				}
			}

		break;
	}




	//Fermeture de la socket joueur
	shutdown(sock, SHUT_RDWR);
	close(sock); 

  return 0;
}



  

  
