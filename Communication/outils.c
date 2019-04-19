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


TCoupReq construireCoup(int socket,TSensTetePiece sens){
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
	coupReq.numPartie = 1;
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



TCoupRep validerAdvers(int sock,TCoupReq coupReqAdversaire){

	//Reception de la reponse validation coup joué par adversaire
	TCoupRep coupRepAdv;
	int err;

	err = recv(sock, &coupRepAdv, sizeof(TCoupRep), 0);
	if (err <= 0) {
		perror("(joueur) erreur dans la reception du coup adversaire");
		shutdown(sock, SHUT_RDWR); close(sock);
		exit(-11);
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
		if (coupRepAdv.validCoup != CONT)
		{
			printf("perdu \n");
		}
	}

	err = recv(sock, &coupReqAdversaire, sizeof(TCoupReq), 0);
	if (err <= 0) {
		perror("(joueur) erreur dans la reception du coup adversaire");
		shutdown(sock, SHUT_RDWR); close(sock);
		exit(-11);
	}
    return coupRepAdv;

}