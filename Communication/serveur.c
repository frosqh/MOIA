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
#include "validation.h"

int main(int argc, char **argv){
	int  socketConnexion,        /* descripteur socket connexion */
       joueur1,
       joueur2,       /* descripteur socket transmission */
       port = 0,            /* numero de port */
       sizeAddr,        /* taille de l'adresse d'une socket */
       err,	            /* code d'erreur */
       boucle1 = 0;
    struct sockaddr_in addClient;	/* adresse de la socket client connectee */   
    bool validation = true;    // utilisation de la validation
    bool timeout = true;       // utilisation de la limite deu temps
    bool fait1=false,fait2=false;
    char* time = "--noTimeout";
    char* valid = "--noValid";
    TPartieReq requetePartie1,requetePartie2;
    TPartieRep reponsePartie1,reponsePartie2;
    fd_set  readSet; 


    //verification des arguments 
       
    if (argc >= 2 || argc <= 4) {
    	if(argc == 2) port  = atoi(argv[1]);
  		
  		else if(argc == 3 && ( !(strcmp(argv[1],time)) || !(strcmp(argv[1],valid)) )) {
  			port  = atoi(argv[2]);

  			if (!(strcmp(argv[1],time))) timeout = false;  				
  			if (!(strcmp(argv[1],valid))) validation = false;
  	
  		}
  		else if(argc == 4 && (!(strcmp(argv[1],time)) || !(strcmp(argv[1],valid))) &&
  		 (!(strcmp(argv[2],time)) || !(strcmp(argv[2],valid))) && (strcmp(argv[1],argv[2]))){
  			port  = atoi(argv[3]);
  			if (!(strcmp(argv[1],time)) || !(strcmp(argv[2],time))) timeout = false;
  			if (!(strcmp(argv[1],valid)) || !(strcmp(argv[2],valid))) validation = false;
  				
  		}
  		else{
  			printf("usage : ./serveur [--noValid|--noTimeout] no_port\n");
  			return -1;
  		}
    
  	}
  	else{
  		printf("usage : ./serveur [--noValid|--noTimeout] no_port\n");
  		return -2;
  	}

  	if (timeout && validation ) printf("Starting ./serveur on port %d with validation and timeout \n",port);
  	else if (!timeout && validation) printf("Starting ./serveur on port %d with validation and  no timeout \n",port);
  	else if (timeout && !validation) printf("Starting ./serveur on port %d with no validation and timeout \n",port);
  	else if (!timeout && !validation) printf("Starting ./serveur on port %d with no validation and  no timeout \n",port);


  	//Création de la socket de connexion :

  	socketConnexion = socketServeur(port);

  	//Création des sockets de transmissions (une pour chaque joueur)

  	

	sizeAddr = sizeof(struct sockaddr_in);
	
	joueur1 = accept(socketConnexion,(struct sockaddr *)&addClient,(socklen_t *)&sizeAddr);
	if (joueur1 < 0) {
	   	perror("(serveur) erreur sur accept joueurSud");
		return -5;
  	}

  	joueur2 = accept(socketConnexion,(struct sockaddr *)&addClient,(socklen_t *)&sizeAddr);
	if (joueur2 < 0) {
    	perror("(serveur) erreur sur accept joueurSud");
    	return -5;
  	}

	FD_ZERO(&readSet);
	FD_SET(joueur1,&readSet);
	FD_SET(joueur2,&readSet);

	//Utilisation du multiplexage
	err = select(FD_SETSIZE, &readSet, NULL, NULL, NULL);
    if (err < 0) {
      	perror("(serveurSelect) erreur dans select");
      	shutdown(socketConnexion, SHUT_RDWR); close(socketConnexion);
    return 5;
    }

    //Traitement de requetes Partie
    while(boucle1 < 2){
    	FD_ZERO(&readSet);
		FD_SET(joueur1,&readSet);
		FD_SET(joueur2,&readSet);
	    if (FD_ISSET(joueur1,&readSet)!=0 && !fait1)
	    {
	    	//reception pour joueur 1
	    	err = recv(joueur1, &requetePartie1, sizeof(TPartieReq), 0);
			if (err <= 0) {
				perror("(clientTCP) erreur dans la reception");
				shutdown(joueur1, SHUT_RDWR); close(joueur1);
			return -4;
			}
			printf("Recu demande partie : nom %s \t\t/ sens SUD\n",requetePartie1.nomJoueur);
			if (boucle1==0) reponsePartie1.validSensTete = OK;
			else reponsePartie1.validSensTete = KO;

			strncpy(reponsePartie1.nomAdvers, "bidon2", T_NOM);
			reponsePartie1.err = 0;
			err = send(joueur1, &reponsePartie1, sizeof(TPartieRep), 0);
    		if (err <= 0){
      			perror("(client) erreur sur le send");
      			shutdown(joueur1, SHUT_RDWR); close(joueur1);
      		return -3;
    		}
	    	boucle1++;
	    	fait1 = true;
	    }
	    printf("on traite la deuxieme maintenant\n");
	    if (FD_ISSET(joueur2,&readSet)!=0 && !fait2)
	    {
	    	err = recv(joueur2, &requetePartie2, sizeof(TPartieReq), 0);
			if (err <= 0) {
				perror("(clientTCP) erreur dans la reception");
				shutdown(joueur2, SHUT_RDWR); close(joueur2);
			return -4;
			}
			printf("Recu demande partie : nom %s \t\t/ sens SUD\n",requetePartie2.nomJoueur);
			if (boucle1==0) reponsePartie2.validSensTete = OK;
			else reponsePartie2.validSensTete = KO;
			strncpy(reponsePartie2.nomAdvers, "bidon1", T_NOM);
			reponsePartie2.err = 0;
			err = send(joueur2, &reponsePartie2, sizeof(TPartieRep), 0);
    		if (err <= 0){
      			perror("(client) erreur sur le send");
      			shutdown(joueur2, SHUT_RDWR); close(joueur2);
      		return -3;
    		}
	    	boucle1++;
	    	fait2 = true;
	    }
	}	








	return 0;
}