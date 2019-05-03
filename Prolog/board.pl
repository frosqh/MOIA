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
removePieceFromGrid(T,1,[[[T,P]|G],IG,C1,C2],[G,IG,C1,[P|C1]]).
removePieceFromGrid(T,1,[[[T2,P2]|G],IG,C1,C2],[NG,IG,C3,C4]):-
	T \= T2,
	removePieceFromGrid(T,1,[G,IG,C1,C2],[NNG,IG,C3,C4]),
	NG = [[T2,P2]|NNG].
removePieceFromGrid(T,-1,[IG,[[T,P]|G],C1,C2],[IG,G,[P|C1],C2]).
removePieceFromGrid(T,-1,[IG,[[T2,P2]|G],C1,C2],[IG,NG,C3,C4]):-
	T \= T2,
	removePieceFromGrid(T,-1,[IG,G,C1,C2],[IG,NNG,C3,C4]),
	NG = [[T2,P2]|NNG].


%:-hasWin/2
%Vérifie si un joueur a gagné la partie (no si aucune victoire)
%hasWin(G,J) : 
				%G = Grille de jeu
				%J = Joueur gagnant [O]
hasWin([[],_,_,_],-1).
hasWin([_,[],_,_],1).
hasWin([_,[[_,N]|G],_,_], 1):-
	\+ piece(N,koropokkuru),
	hasWin([[1],G,[],[]], 1).
hasWin([[[_,N]|G],_,_,_], -1):-
	\+ piece(N,koropokkuru),
	hasWin([G,[1],[],[]], -1).