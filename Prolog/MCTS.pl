%:-simu/5
%Effectue une simulation d'une partie
%simu(Grid, Turn, Player, MoveList, Winner) :
					%Grid = La grille de jeu au tour Turn
					%Turn = Le numéro du tour actuel
					%Player = Le joueur dont c'est le tour
					%MoveList = Une structure correspondant au stockage de l'arbre des rollouts de la forme
						%TODO À modifier si usage des tables de commutations
							%[[Move, Throughs, WinP1, WinP2, Draw, ValueUCB1, KeyList, MoveList]*] avec : 
									%Move de la forme [[X,Y],[X2,Y2]] symbolisant un coup traité
									%Throughs = Le nombre de simulation passant par ce noeud
									%WinP1 = Le nombre de victoires de P1 après être passé par ce noeud
									%WinP2 = Le nombre de victoires de P2 après être passé par ce noeud
									%Draw  = Le nombre d'égalité après être passé par ce noeud
									%ValueUCB1 = La valeur UCB1 du noeud, afin d'optimiser l'expansion des rollouts
									%KeyList = La liste des moves fils déjà traités (pour accelérer le traitement UCT)
									%MoveList = L'arbre des rollouts à partir de ce  noeud



simu(_, Turn, _, _,_, 0) :-
	Turn > 60.

simu(Grid, _, _, _,_, Y):-
	hasWin(Grid,Y).

simu(Grid, Turn, Player, MoveList, NewMoveList,Winner):-
	allAvailableMoves(Grid,Player,Moves),
	toExpand(Moves,MoveList,NewMoveList,Winner).
%Ici, on regarde dans notre moveList si on a un move possible qui n'est pas traité à partir de notre Grid
%Si c'est le cas, on choisi un de ces moves au random
%Sinon, on utilise les différentes valeurs UCB1 pour choisir le noeud à traiter
%Puis, on effectue la simulation au niveau plus bas et on actualise enfin notre bordel avec le through and co ...(Il va falloir séparer la vérification et l'application du move)


allAvailableMoves(G, 1, LR) :-
	G = [P1,_],
	pieceAvailableMoves(P1, G, 1, LR).

pieceAvailableMoves([],_,_,[]).
pieceAvailableMoves([P|L], G, J, LR):-
	findall(T, availableMovePiece(P,J,G,T), LPR),
	createMoves(LPR,P,LLR),
	pieceAvailableMoves(L,G,J,LTR),
	append(LLR,LTR,LR).

createMoves([],_,[]).
createMoves([T|L],P,LR):-
	createMoves(L,P,LLR),
	append([[P,T]],LLR,LR).

toExpand(Moves, MoveList, NewMoveList, MoveToExpand) :-
	getKeyList(MoveList, KeyList),
	notAlreadyTreated(Moves, KeyList, NotTreatedMoves),
	NotTreatedMoves \= [],
	!, % Pour ne pas recalculer notAlreadyTreated
	length(NotTreatedMoves, Size),
	Upper is Size-1,
	random(0,Upper,Index),
	nth0(Index, NotTreatedMoves,  MoveToExpand),
	display(MoveToExpand),
	addMove(MoveToExpand, MoveList, NewMoveList).

toExpand(Moves, MoveList, MoveList, MoveToExpand) :-
	maxValue(MoveList, BigMoveToExpand),
	getMove(BigMoveToExpand, MoveToExpand).

notAlreadyTreated([], KeyList, []).

notAlreadyTreated([M|Moves], KeyList, NotTreatedMoves):-
	\+ member(M, KeyList),
	!,
	notAlreadyTreated(Moves,KeyList,TmpNotTreatedMoves),
	append([M], TmpNotTreatedMoves, NotTreatedMoves).

notre([M|Moves], KeyList, NotTreatedMoves):-
	notAlreadyTreated(Moves,KeyList,NotTreatedMoves).
	