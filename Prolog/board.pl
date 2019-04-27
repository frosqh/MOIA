%:-initialGrid/1
%Définit la grille initiale
initialGrid(G):-
	initialGrid(1,P1G),
	initialGrid(-1,P2G),
	G = [P1G,P2G].

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
updateGrid([[P|G],IG],P,1,NP,[[NP|G],IG]).
updateGrid([[P2|G],IG],P,1,NP,GR):-
	P \= P2,
	updateGrid([G,IG],P,1,NP,[G1,G2]),
	GR = [[P2|G1],G2].


updateGrid([IG,[P|G]],P,-1,NP,[IG,[NP|G]]).
updateGrid([IG,[P2|G]],P,-1,NP,GR):-
	P \= P2,
	updateGrid([IG,G],P,-1,NP,[G1,G2]),
	GR = [G1,[P2|G2]].

%:-removePieceFromGrid/4
%Retire une pièce de la grille de jeu
%removePieceFromGrid(T,J,G,NG) : 
				%T = Coordonnées de la Pièce
				%J = Identifiant du joueur
				%G = Grille de jeu
				%NG = Grille après retrait de la pièce [O]
removePieceFromGrid(T,1,[[[T,_]|G],IG],[G,IG]).
removePieceFromGrid(T,1,[[[T2,P2]|G],IG],[NG,IG]):-
	T \= T2,
	removePieceFromGrid(T,1,[G,IG],[NNG,IG]),
	NG = [[T2,P2]|NNG].
removePieceFromGrid(T,-1,[IG,[[T,_]|G]],[IG,G]).
removePieceFromGrid(T,-1,[IG,[[T2,P2]|G]],[IG,NG]):-
	T \= T2,
	removePieceFromGrid(T,-1,[IG,G],[IG,NNG]),
	NG = [[T2,P2]|NNG].


%:-hasWin/2
%Vérifie si un joueur a gagné la partie (no si aucune victoire)
%hasWin(G,J) : 
				%G = Grille de jeu
				%J = Joueur gagnant [O]
hasWin([[],_],-1).
hasWin([_,[]],1).
hasWin([_,[[_,N]|G]], 1):-
	\+ piece(N,koropokkuru),
	hasWin([[1],G], 1).
hasWin([[[_,N]|G],_], -1):-
	\+ piece(N,koropokkuru),
	hasWin([G,[1]], -1).