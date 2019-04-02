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


int main(int argc, char **argv){
	int sock,port,err;                 
	char* nomMachServ;       
	TPartieReq partieReq;
	TPartieRep partieRep;
	char sens;
	char sensAccorde;		
	
	
	 
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
    scanf("%c", &sens);
    
    if (sens == 's')
    {
    	partieReq.piece = SUD;
    	
    }
    if (sens == 'n')
    {
    	partieReq.piece = NORD;
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

	if (partieRep.validSensTete == NORD)
	{
		sensAccorde = 'n';
	}
	else{
		sensAccorde = 's';
	}





	shutdown(sock, SHUT_RDWR);
	close(sock);



    



    
    
    
  

  return 0;
}



  

  
