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
	TCoupReq coupReq;
	TCoupReq coupReqAdversaire;
	char sensDemande;
	TSensTetePiece sensAccorde;
	TSensTetePiece sens;
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
		perror("(joueur) erreur dans la reception");
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
	printf("******************************* \n");
	printf("************ Partie 1 ********* \n");
	printf("******************************* \n");

	switch (sensAccorde){
		//on commence en premier
		
		case SUD:
			printf("******************************* \n");
			printf("** C'est à vous de commencer ** \n");
			printf("******************************* \n");
			while(true){
			
				//Construction d'un coup (Java)
				printf("Construction du coup en cours \n");
			    coupReq = construireCoup(sockJava,SUD,1);
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

				if (coupRep.err == ERR_OK && coupRep.validCoup != TRICHE && coupRep.propCoup == CONT)
				{
					printf("Coup valide !\n");
				}
				else{
					printf("Coup non valide partie finie !\n");
					break;
				}
				printf("Coup joué \n");
				//Reception de la reponse validation coup joué par adversaire
				printf("Attente coup adversaire...\n");

				err = recv(sock, &coupRepAdv, sizeof(TCoupRep), 0);
				if (err <= 0) {
					perror("(joueur) erreur dans la reception du coup adversaire");
					shutdown(sock, SHUT_RDWR); close(sock);
					break;
				}

				if (coupRepAdv.err == ERR_OK && coupRepAdv.validCoup != TRICHE && coupRepAdv.propCoup == CONT)
				{
					printf("Coup valide !\n");
				}
				else{
					if (coupRepAdv.err == ERR_COUP)
					{
						printf("coup erroné \n");
					}
					if (coupRepAdv.validCoup == TRICHE)
					{
						printf("coup triché \n");
					}
					if (coupRepAdv.propCoup != CONT)
					{
						printf("perdu \n");
					}
				}

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

				if (coupRepAdv.err == ERR_OK && coupRepAdv.validCoup != TRICHE && coupRepAdv.propCoup == CONT)
				{
					printf("Coup valide !\n");
				}
				else{
					if (coupRepAdv.err == ERR_COUP)
					{
						printf("coup erroné \n");
					}
					if (coupRepAdv.validCoup == TRICHE)
					{
						printf("coup triché \n");

					}
					if (coupRepAdv.propCoup != CONT)
					{
						printf("perdu \n");
					}
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

				if (coupRep.err == ERR_OK && coupRep.validCoup != TRICHE && coupRep.propCoup == CONT)
				{
					printf("Coup valide !\n");
				}
				else{
					printf("Coup non valide partie finie !\n");
					break;
				}
				printf("Coup joué \n"); 			
			}
		break;
	}

	printf("******************************* \n");
	printf("************ Partie 2 ********* \n");
	printf("******************************* \n");
	printf("\n");

	switch (sensAccorde){
		//on commence en premier
		
		case NORD:
			printf("******************************* \n");
			printf("** C'est à vous de commencer ** \n");
			printf("******************************* \n");
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

				if (coupRep.err == ERR_OK && coupRep.validCoup != TRICHE && coupRep.propCoup == CONT)
				{
					printf("Coup valide !\n");
				}
				else{
					printf("Coup non valide partie finie !\n");
					break;
				}
				printf("Coup joué \n");
				//Reception de la reponse validation coup joué par adversaire
				printf("Attente coup adversaire...\n");

				err = recv(sock, &coupRepAdv, sizeof(TCoupRep), 0);
				if (err <= 0) {
					perror("(joueur) erreur dans la reception du coup adversaire");
					shutdown(sock, SHUT_RDWR); close(sock);
					break;
				}

				if (coupRepAdv.err == ERR_OK && coupRepAdv.validCoup != TRICHE && coupRepAdv.propCoup == CONT)
				{
					printf("Coup valide !\n");
				}
				else{
					if (coupRepAdv.err == ERR_COUP)
					{
						printf("coup erroné \n");
					}
					if (coupRepAdv.validCoup == TRICHE)
					{
						printf("coup triché \n");
					}
					if (coupRepAdv.propCoup != CONT)
					{
						printf("perdu \n");
					}
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

				if (coupRepAdv.err == ERR_OK && coupRepAdv.validCoup != TRICHE && coupRepAdv.propCoup == CONT)
				{
					printf("Coup valide !\n");
				}
				else{
					if (coupRepAdv.err == ERR_COUP)
					{
						printf("coup erroné \n");
					}
					if (coupRepAdv.validCoup == TRICHE)
					{
						printf("coup triché \n");
					}
					if (coupRepAdv.propCoup != CONT)
					{
						printf("perdu \n");
					}
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

				if (coupRep.err == ERR_OK && coupRep.validCoup != TRICHE && coupRep.propCoup == CONT)
				{
					printf("Coup valide !\n");
				}
				else{
					printf("Coup non valide partie finie !\n");
					break;
				}
				printf("Coup joué \n"); 			
			}
		break;
	}




	//Fermeture de la socket joueur
	shutdown(sock, SHUT_RDWR);
	close(sock); 

  return 0;
}



  

  
