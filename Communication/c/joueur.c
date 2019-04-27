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
#include "outils.h"

//// 18/04/2019 
//// version beta joueur
//// testé : envoi et reception de réponse de types coup et partie (OK)
//// communication avec java




int main(int argc, char **argv){
	int sock,port,err,sockJava,portJava;                 
	char* nomMachServ;       
	TPartieReq partieReq;
	TPartieRep partieRep;
	TCoupReq coupReq,coupReqAdversaire;
	TSensTetePiece sensAccorde;
	TCoupRep coupRep,coupRepAdv;
		 
	/* verification des arguments */
	if (argc != 4) {
	   printf("usage : %s nom/IPServ port-arbitre port-moteur-java\n", argv[0]);
	    return -1;
	}
	  
	nomMachServ = argv[1];
	port = atoi(argv[2]);
	portJava = atoi(argv[3]);

	//etablir en connexion avec l'arbitre
	sock = socketClient(nomMachServ, port);
	if (sock < 0) {
	    perror("(client) erreur sur la creation de socket");
	    return -2;
	}
	//etablir une connexion avec le moteur java
	sockJava = socketClient(nomMachServ, portJava);
	if (sockJava < 0) {
	    perror("(client) erreur sur la creation de socket");
	    return -2;
	}



	//Envoi de la requete Partie (fonction : TSensTetePiece debutPartie(int sock,char nomJoueur[T_NOM],TSensTetePiece sensDemande) )
	
	sensAccorde = debutPartie(sock, partieReq, partieRep);
	

	printf("\n\n");
	printf("******************************* \n");
	printf("********** Partie aller ******* \n");
	printf("******************************* \n\n\n");

	switch (sensAccorde){
		//on commence en premier
		
		case SUD:
			
			printf("** C'est à vous de commencer ** \n");
			
			while(true){
			
				//Construction d'un coup (Java)
				printf("Construction du coup en cours \n");
			    coupReq = construireCoup(sockJava,SUD,1);
			    //de preference le send à mettre dans la fonction construireCoup
			    err = send(sock, &coupReq, sizeof(TCoupReq), 0);
				if (err <= 0){
						perror("(joueur) erreur sur le send coup");
						shutdown(sock, SHUT_RDWR); close(sock);
					break;
				}
    			printf("Coup construit et envoyé \n");


			    //reception de la validation
				err = recv(sock, &coupRep, sizeof(TCoupRep), 0);
				if (err <= 0) {
					perror("(joueur) erreur dans la reception de la validation coup");
					shutdown(sock, SHUT_RDWR); close(sock);
					break;
				}

				if (coupRep.err == ERR_OK && coupRep.validCoup != TRICHE && coupRep.propCoup == CONT) printf("Coup valide !\n");
				else{
					if (coupRep.err == ERR_COUP) printf("Coup non valide !\n");
					if (coupRep.validCoup == TRICHE) printf("Coup triché !\n");
					if (coupRep.propCoup == GAGNE) printf("Vous avez gagné cette partie !\n");
					if (coupRep.propCoup == PERDU) printf("Vous avez perdu cette partie...\n");
					if (coupRep.propCoup == NUL) printf("Match nul, il n'y a pas de vianqueur \n");
					
					break;
				} 		
				
				//Reception de la reponse validation coup joué par adversaire
			    //debut
				printf("Attente coup adversaire...\n");
				err = recv(sock, &coupRepAdv, sizeof(TCoupRep), 0);
				if (err <= 0) {
					perror("(joueur) erreur dans la reception du coup adversaire");
					shutdown(sock, SHUT_RDWR); close(sock);
					break;
				}

				if (coupRepAdv.err == ERR_OK && coupRepAdv.validCoup != TRICHE && coupRepAdv.propCoup == CONT) printf("Coup adversaire valide !\n");
				else{
					if (coupRepAdv.err == ERR_COUP) printf("Coup adversaire non valide !\n");
					if (coupRepAdv.validCoup == TRICHE) printf("Coup adversaire triché !\n");
					if (coupRepAdv.propCoup == GAGNE) printf("L'adversaire a gagné cette partie...\n");
					if (coupRepAdv.propCoup == PERDU) printf("L'adversaire a perdu cette partie !\n");
					if (coupRepAdv.propCoup == NUL) printf("Match nul, il n'y a pas de vianqueur \n");
					break;
				}
				//fin

				err = recv(sock, &coupReqAdversaire, sizeof(TCoupReq), 0);
				if (err <= 0) {
					perror("(joueur) erreur dans la reception du coup adversaire");
					shutdown(sock, SHUT_RDWR); close(sock);
					break;
				}

				
				
			}

		break;

		case NORD:
			printf("******************************* \n");
			printf("*** Vous jouez en deuxième **** \n");
			printf("******************************* \n");
			//////**************************************
			while(true){
				printf("Attente coup adversaire...\n");
				err = recv(sock, &coupRepAdv, sizeof(TCoupRep), 0);
				if (err <= 0) {
					perror("(joueur) erreur dans la reception du coup adversaire");
					shutdown(sock, SHUT_RDWR); close(sock);
					break;
				}

				if (coupRepAdv.err == ERR_OK && coupRepAdv.validCoup != TRICHE && coupRepAdv.propCoup == CONT) printf("Coup adversaire valide !\n");
				else{
					if (coupRepAdv.err == ERR_COUP) printf("Coup adversaire non valide !\n");
					if (coupRepAdv.validCoup == TRICHE) printf("Coup adversaire triché !\n");
					if (coupRepAdv.propCoup == GAGNE) printf("L'adversaire a gagné cette partie...\n");
					if (coupRepAdv.propCoup == PERDU) printf("L'adversaire a perdu cette partie !\n");
					if (coupRepAdv.propCoup == NUL) printf("Match nul, il n'y a pas de vianqueur \n");
					break;
				}
				err = recv(sock, &coupReqAdversaire, sizeof(TCoupReq), 0);
				if (err <= 0) {
					perror("(joueur) erreur dans la reception du coup adversaire");
					shutdown(sock, SHUT_RDWR); close(sock);
					break;
				}
				//Construction d'un coup
				printf("Construction du coup en cours \n");
				coupReq = construireCoup(sockJava,NORD,1);
				err = send(sock, &coupReq, sizeof(TCoupReq), 0);
				if (err <= 0){
						perror("(joueur) erreur sur le send coup");
						shutdown(sock, SHUT_RDWR); close(sock);
					break;
				}
				printf("Coup construit et envoyé\n");
    			//envoi du coup et reception de la reponse
				err = recv(sock, &coupRep, sizeof(TCoupRep), 0);
				if (err <= 0) {
					perror("(joueur) erreur dans la reception de la validation coup");
					shutdown(sock, SHUT_RDWR); close(sock);
					break;
				}

				if (coupRep.err == ERR_OK && coupRep.validCoup != TRICHE && coupRep.propCoup == CONT) printf("Coup valide !\n");
				else{
					if (coupRep.err == ERR_COUP) printf("Coup non valide !\n");
					if (coupRep.validCoup == TRICHE) printf("Coup triché !\n");
					if (coupRep.propCoup == GAGNE) printf("Vous avez gagné cette partie !\n");
					if (coupRep.propCoup == PERDU) printf("Vous avez perdu cette partie...\n");
					if (coupRep.propCoup == NUL) printf("Match nul, il n'y a pas de vianqueur \n");
					
					break;
				} 		
				 			
			}
		break;
	}
	printf("\n\n");
	printf("******************************* \n");
	printf("************ Partie 2 ********* \n");
	printf("******************************* \n\n");

	switch (sensAccorde){
		//on commence en premier
		
		case NORD:
			printf("******************************* \n");
			printf("** C'est à vous de commencer ** \n");
			printf("******************************* \n\n");
			while(true){
			
				//Construction d'un coup (Java)
				printf("Construction du coup en cours \n");
			    coupReq = construireCoup(sockJava,NORD,2);
			    err = send(sock, &coupReq, sizeof(TCoupReq), 0);
				if (err <= 0){
						perror("(joueur) erreur sur le send coup");
						shutdown(sock, SHUT_RDWR); close(sock);
					break;
				}
    			printf("Coup construit et envoyé \n");
			    //envoi du coup et reception de la reponse
				err = recv(sock, &coupRep, sizeof(TCoupRep), 0);
				if (err <= 0) {
					perror("(joueur) erreur dans la reception de la validation coup");
					shutdown(sock, SHUT_RDWR); close(sock);
					break;
				}

				if (coupRep.err == ERR_OK && coupRep.validCoup != TRICHE && coupRep.propCoup == CONT) printf("Coup valide !\n");
				else{
					if (coupRep.err == ERR_COUP) printf("Coup non valide !\n");
					if (coupRep.validCoup == TRICHE) printf("Coup triché !\n");
					if (coupRep.propCoup == GAGNE) printf("Vous avez gagné cette partie !\n");
					if (coupRep.propCoup == PERDU) printf("Vous avez perdu cette partie...\n");
					if (coupRep.propCoup == NUL) printf("Match nul, il n'y a pas de vianqueur \n");
					
					break;
				} 		
				//Reception de la reponse validation coup joué par adversaire
				printf("Attente coup adversaire...\n");

				err = recv(sock, &coupRepAdv, sizeof(TCoupRep), 0);
				if (err <= 0) {
					perror("(joueur) erreur dans la reception du coup adversaire");
					shutdown(sock, SHUT_RDWR); close(sock);
					break;
				}

				if (coupRepAdv.err == ERR_OK && coupRepAdv.validCoup != TRICHE && coupRepAdv.propCoup == CONT) printf("Coup adversaire valide !\n");
				else{
					if (coupRepAdv.err == ERR_COUP) printf("Coup adversaire non valide !\n");
					if (coupRepAdv.validCoup == TRICHE) printf("Coup adversaire triché !\n");
					if (coupRepAdv.propCoup == GAGNE) printf("L'adversaire a gagné cette partie...\n");
					if (coupRepAdv.propCoup == PERDU) printf("L'adversaire a perdu cette partie !\n");
					if (coupRepAdv.propCoup == NUL) printf("Match nul, il n'y a pas de vianqueur \n");
					break;
				}

				err = recv(sock, &coupReqAdversaire, sizeof(TCoupReq), 0);
				if (err <= 0) {
					perror("(joueur) erreur dans la reception du coup adversaire");
					shutdown(sock, SHUT_RDWR); close(sock);
					break;
				}
				
				
			}

		break;

		case SUD:
			printf("******************************* \n");
			printf("*** Vous jouez en deuxième **** \n");
			printf("******************************* \n");
			//////**************************************
			while(true){
				printf("Attente coup adversaire...\n");
				err = recv(sock, &coupRepAdv, sizeof(TCoupRep), 0);
				if (err <= 0) {
					perror("(joueur) erreur dans la reception du coup adversaire");
					shutdown(sock, SHUT_RDWR); close(sock);
					break;
				}

				if (coupRepAdv.err == ERR_OK && coupRepAdv.validCoup != TRICHE && coupRepAdv.propCoup == CONT) printf("Coup adversaire valide !\n");
				else{
					if (coupRepAdv.err == ERR_COUP) printf("Coup adversaire non valide !\n");
					if (coupRepAdv.validCoup == TRICHE) printf("Coup adversaire triché !\n");
					if (coupRepAdv.propCoup == GAGNE) printf("L'adversaire a gagné cette partie...\n");
					if (coupRepAdv.propCoup == PERDU) printf("L'adversaire a perdu cette partie !\n");
					if (coupRepAdv.propCoup == NUL) printf("Match nul, il n'y a pas de vianqueur \n");
					break;
				}

				err = recv(sock, &coupReqAdversaire, sizeof(TCoupReq), 0);
				if (err <= 0) {
					perror("(joueur) erreur dans la reception du coup adversaire");
					shutdown(sock, SHUT_RDWR); close(sock);
					break;
				}
				//Construction d'un coup
				printf("Construction du coup en cours \n");
				coupReq = construireCoup(sockJava,SUD,2);
				err = send(sock, &coupReq, sizeof(TCoupReq), 0);
				if (err <= 0){
						perror("(joueur) erreur sur le send coup");
						shutdown(sock, SHUT_RDWR); close(sock);
					break;
				}
				printf("Coup construit et envoyé\n");
    			//envoi du coup et reception de la reponse 
				err = recv(sock, &coupRep, sizeof(TCoupRep), 0);
				if (err <= 0) {
					perror("(joueur) erreur dans la reception de la validation coup");
					shutdown(sock, SHUT_RDWR); close(sock);
					break;
				}

				if (coupRep.err == ERR_OK && coupRep.validCoup != TRICHE && coupRep.propCoup == CONT) printf("Coup valide !\n");
				else{
					if (coupRep.err == ERR_COUP) printf("Coup non valide !\n");
					if (coupRep.validCoup == TRICHE) printf("Coup triché !\n");
					if (coupRep.propCoup == GAGNE) printf("Vous avez gagné cette partie !\n");
					if (coupRep.propCoup == PERDU) printf("Vous avez perdu cette partie...\n");
					if (coupRep.propCoup == NUL) printf("Match nul, il n'y a pas de vianqueur \n");

					break;
				} 			
			}
		break;
	}

	printf("Fermeture des sockets (c et java)\n");
	shutdown(sockJava, SHUT_RDWR); 
	close(sockJava);
	shutdown(sock, SHUT_RDWR);
	close(sock); 

  return 0;
}



  

  
