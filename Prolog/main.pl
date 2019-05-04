:-use_module(library(plunit)).
:-use_module(library(random)).
:-use_module(library(lists)).
:-use_module(library(system)).
:-set_prolog_flag(toplevel_print_options,[max_depth(0)]).
:-set_prolog_flag(double_quotes, chars).
:-['./movePiece.pl'].
:-['./board.pl'].
:-['./util.pl'].
:-['./MoveNode.pl'].
:-['./MCTS.pl'].

display(Depart,R):-
	statistics(runtime,[Fin,_]),
	Temps is (Fin-Depart)//1000,
	Minute is Temps//60,
	Seconde is Temps-(60*Minute),
	MiliSeconde is (Fin-Depart) - Temps*1000,
	nl,
	write('R = '),
	write(R),
	nl,
	write('Execution Time: '),
	write(Minute),
	write(' min '),
	write(Seconde),
	write(' s'),
	write(MiliSeconde),
	write(' ms'),
	nl,nl.



test(R) :-
	initialGrid(G),
	piece(Oni,oni),
	P = [[0,0],Oni],
	movePiece(P,1,G,R).

test2(R):-
	initialGrid(G),
	piece(Oni,oni),
	P = [[1,0],Oni],
	movePiece(P,1,G,R).

test3(R):-
	initialGrid(G),
	piece(Kodama,kodama),
	P = [[2,2],Kodama],
	movePiece(P,1,G,GR),
	write(GR),nl,
	availableDrop(Kodama, 1, GR, R).

test4(R):-
	initialGrid(G),
	hasWin(G,R).

test5(R):-
	piece(Kodama,kodama),
	G = [
				[
					[[1,2],Kodama]
				],
				[
					[[2,3],Kodama]
				]
			],
	hasWin(G,R).

test6(R):-
	initialGrid(G),
	allAvailableMoves(G,1,R).

test7:-
	initialGrid(G),
	statistics(runtime,[Depart,_]),
	simu(G,0,1,[0,0,0,0,0,0,[],[]],1,MoveList,Winner),
	%getValue(MoveList, Value),
	%updateValue(MoveList, 5, D),
	simu(G,0,1,MoveList,2,_,_).

test8(R):-
	initialGrid(G),
	statistics(runtime,[Depart,_]),
	simuUntilTimeout(Depart, [0,0,0,0,0,0,[],[]], G, 0, 1, R),
	display(Depart,R),
	nl.

test9(R):-
	initialGrid(G),
	allAvailableMoves(G,1,L),
	L = [M1|L1],
	KeyList = [M1],
	notAlreadyTreated(L,KeyList,R),
	length(R,A),
	write(A).

%G : Etat de la grille ([] si début)
%M : Etat de la liste de couts ([] si début)
%J : Joueur
%MJ : Mon idendité
%T : Tour
%R : Meilleur Move
%TODO Déterminer mon joueur (Avec une variable MJ par exemple)
%Pour l'instant, recherche d'un coup gagnant à grand coup 
testJasper([],[],J,MJ,0,R):-
	!,
	initialGrid(G),
	statistics(runtime,[Depart,_]),
	simuUntilTimeout(Depart, [0,0,0,0,0,0,[],[]], G, 0, J,MJ, LR),
	display(Depart, 0),
	getMoveList(LR,L),
	display(Depart, 0),
	getMaxWinRate(L,P),
	display(Depart, 0),
	getMove(P,R).
testJasper(G,M,J,MJ,T,R):-
	statistics(runtime,[Depart,_]),
	simuUntilTimeout(Depart,M,G,T,J,MJ,LR),
	display(Depart, 0),
	getMoveList(LR,L),
	getMaxWinRate(L,P),
	getMove(P,R).

getMaxWinRate([M],M):-!.
getMaxWinRate([M|L],R):-
	getMaxWinRate(L,T),
	getWinP1(M,WM),
	getThroughs(M,TM),
	getWinP1(T,WT),
	getThroughs(T,TT),
	getMaxMove(M,WM/TM,T,WT/TT,R).
	
genFile(J,MJ):-
	initialGrid(G),
	statistics(runtime,[Depart,_]),
	simuUntilTimeout(Depart, [0,0,0,0,0,0,[],[]], G, 0, J, MJ, LR),
	write(LR).