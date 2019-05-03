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
	P2 = [[1,2],Kodama],
	movePiece(P2,1,GR,R).

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