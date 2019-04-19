#include <stdbool.h>

TCoupReq construireCoup(int socket,TSensTetePiece sens);
TCoupRep validerCoup(int sock,TCoupReq coupReq,TSensTetePiece sens);
TCoupRep validerAdvers(int sock,TCoupReq coupReqAdversaire);