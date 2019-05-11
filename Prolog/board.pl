%:-initialGrid/1
%Définit la grille initiale
initialGrid(G):-
	initialGrid(1,P1G),
	initialGrid(-1,P2G),
	G = [P1G,P2G,[],[]].

gridCase([A,B]):-
	member(A,[0,1,2,3,4]),
	member(B,[0,1,2,3,4,5]).

%:-initialGrid/2
%Définit la grille initiale pour le joueur donné.
%initialGrid(J,G) : 
				%J = Identifiant du joueur
				%G = Grille initiale pour ce joueur [O]
initialGrid(1,PG):-
	piece(Kodama,kodama),
	piece(Oni,oni),
	piece(Kirin,kirin),
	piece(Koro,koropokkuru),
	PG = [
					[[1,2],Kodama],
					[[2,2],Kodama],
					[[3,2],Kodama],
					[[0,0],Oni],
					[[4,0],Oni],
					[[1,0],Kirin],
					[[3,0],Kirin],
					[[2,0],Koro]
	].

initialGrid(-1,PG):-
	piece(Kodama,kodama),
	piece(Oni,oni),
	piece(Kirin,kirin),
	piece(Koro,koropokkuru),
	PG = [
					[[1,3],Kodama],
					[[2,3],Kodama],
					[[3,3],Kodama],
					[[0,5],Oni],
					[[4,5],Oni],
					[[1,5],Kirin],
					[[3,5],Kirin],
					[[2,5],Koro]
	].

%:-updateGrid/5
%Met à jour la grille après le déplacement de P vers NP
%updateGrid(G,P,J,NP,NG) : 
			%G = Grille avant coup joué
			%P = Pièce avant coup joué
			%J = Identifiant du joueur
			%NP = Pièce après coup joué
			%NG = Grille après coup joué [O]
updateGrid([[P|G],IG,C1,C2],P,1,NP,[[NP|G],IG,C1,C2]).
updateGrid([[P2|G],IG,C1,C2],P,1,NP,GR):-
	P \= P2,
	updateGrid([G,IG,C1,C2],P,1,NP,[G1,G2,C3,C4]),
	GR = [[P2|G1],G2,C3,C4].


updateGrid([IG,[P|G],C1,C2],P,-1,NP,[IG,[NP|G],C1,C2]).
updateGrid([IG,[P2|G],C1,C2],P,-1,NP,GR):-
	P \= P2,
	updateGrid([IG,G,C1,C2],P,-1,NP,[G1,G2,C3,C4]),
	GR = [G1,[P2|G2],C3,C4].

dropGrid(N,1,[P1,P2,C1,C2],T,[P,P2,C,C2]):-
	delete(C1,N,C),
	append([[T,N]],P1,P).
dropGrid(N,-1,[P1,P2,C1,C2],T,[P1,P,C1,C]):-
	delete(C2,N,C),
	append([[T,N]],P2,P).
%:-removePieceFromGrid/4
%Retire une pièce de la grille de jeu
%removePieceFromGrid(T,J,G,NG) : 
				%T = Coordonnées de la Pièce
				%J = Identifiant du joueur
				%G = Grille de jeu
				%NG = Grille après retrait de la pièce [O]
removePieceFromGrid(T,1,[[[T,P]|G],IG,C1,C2],[G,IG,C1,[P2|C2]]):-
	downgrade(P,P2).
removePieceFromGrid(T,1,[[[T2,P2]|G],IG,C1,C2],[NG,IG,C3,C4]):-
	T \= T2,
	removePieceFromGrid(T,1,[G,IG,C1,C2],[NNG,IG,C3,C4]),
	NG = [[T2,P2]|NNG].
removePieceFromGrid(T,-1,[IG,[[T,P]|G],C1,C2],[IG,G,[P2|C1],C2]):-
	downgrade(P,P2).
removePieceFromGrid(T,-1,[IG,[[T2,P2]|G],C1,C2],[IG,NG,C3,C4]):-
	T \= T2,
	removePieceFromGrid(T,-1,[IG,G,C1,C2],[IG,NNG,C3,C4]),
	NG = [[T2,P2]|NNG].


downgrade(P,P2):-
	piece(KS, kodama_samourai),
	piece(SO, super_oni),
	member(P,[KS,SO]),
	!,
	P2 is P-1.
downgrade(P,P).

%:-firstHasWin/2
%Vérifie si un joueur a gagné la partie (no si aucune victoire)
%firstHasWin(G,J) : 
				%G = Grille de jeu
				%J = Joueur gagnant [O]
firstHasWin([[],_,_,_],-1).
firstHasWin([_,[],_,_],1).
firstHasWin([_,[[_,N]|G],_,_], 1):-
	\+ piece(N,koropokkuru),
	firstHasWin([[1],G,[],[]], 1).
firstHasWin([[[_,N]|G],_,_,_], -1):-
	\+ piece(N,koropokkuru),
	firstHasWin([G,[1],[],[]], -1).

hasWin([P1,P2,_,_], Opp):-
	%Récupérer les coordonnées du roi de J
	%Regarder tous les moves des pièces de J dans un rayon de 2 du roi + plus le roi (pour tous, il doit y avoir capture possible par -J tout de même)
	%Regarder les moves de toutes les pièces de -J dans un rayon du 1 de nouveau roi
	getKing(P1,P2,J,K),
	getPiecesWithDistance(K,2,P1,P2,J,Ps),
	verifMat(Ps,P1,P2,J),
	Opp is -J.

isCheck([P1, P2, _, _], J):-
	getKing(P1,P2,J,K),
	Opp is -J,
	getPiecesWithDistance(K,sqrt(2),P1,P2,Opp,Ps),
	isAttackable(K, Opp ,Ps).

isAttackable(K, _,[]):-fail,!.
isAttackable(T, J,[P|Ps]):-
	move(P,J,T),
	!.
isAttackable(K, J,[P|Ps]):-
	isAttackable(K,J,Ps).


getKing([[A,Koro]|_],_,1,A):-
	piece(Koro,koropokkuru),!.
getKing([_|P],_,1,K):-
	getKing(P,0,1,K).
getKing(_,[[A,Koro]|_],-1,A):-
	piece(Koro,koropokkuru),!.
getKing(_,[_|P],-1,K):-
	getKing(0,P,-1,K).


getPiecesWithDistance(K,D,[[C,P]|P1],P2,1,Ps):-
	getDistance(K,C,DK),
	D >= DK,!,
	getPiecesWithDistance(K,D,P1,P2,1,Pss),
	append([[C,P]],Pss,Ps).
getPiecesWithDistance(K,D,[_|P1],P2,1,Ps):-
	getPiecesWithDistance(K,D,P1,P2,1,Ps).
getPiecesWithDistance(K,D,[],P2,1,[]).
getPiecesWithDistance(K,D,P1,[[C,P]|P2],-1,Ps):-
	getDistance(K,C,DK),
	D >= DK,!,
	getPiecesWithDistance(K,D,P1,P2,-1,Pss),
	append([[C,P]],Pss,Ps).
getPiecesWithDistance(K,D,P1,[_|P2],-1,Ps):-
	getPiecesWithDistance(K,D,P1,P2,-1,Ps).
getPiecesWithDistance(K,D,P1,[],-1,[]).
verifMat(Ps,P1,P2,J):-
	pieceAvailableMoves(Ps,[P1,P2,[],[]],J,LR),
	execAndVerif(LR, P1, P2, J).

execAndVerif([[P,M] | L], P1, P2, J):-
	actuallyMovePiece(P,J,[P1,P2,_,_],M,[PP1,PP2,_,_]),
	getKing(PP1,PP2,J,K),
	Opp is -J,
	S is sqrt(2)+1/10,
	getPiecesWithDistance(K,S,PP1,PP2,Opp,Ps),
	pieceAvailableMoves(Ps, [PP1, PP2, [], []],Opp,LR),
	verifCapt(LR,K),
	execAndVerif(L,P1,P2,J).

execAndVerif([],_,_,_).

verifCapt([],_):-fail,!.
verifCapt([[_,K]|_],K):-!.
verifCapt([_|L],K):-
	verifCapt(L,K).

getDistance([X,Y],[X2,Y2],D):-
	D is sqrt((X-X2)*(X-X2)+(Y-Y2)*(Y-Y2)).