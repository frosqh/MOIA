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
	simu(G,0,1,1,[0,0,0,0,0,0,[],[]],1,LR,_),
	getThroughs(LR, Throughs),
	getWinP1(LR,WinP1),
	getDraw(LR,Draw),
	write('Throughs : '),write(Throughs),nl,
	write('Win : '),write(WinP1),nl,
	write('Draw : '),write(Draw),nl.

test8(R):-
	initialGrid(G),
	statistics(runtime,[Depart,_]),
	simuUntilTimeout(Depart, [0,0,0,0,0,0,[],[]], G, 0, 1, R),
	display(Depart,R),
	nl.

test9(R):-
	initialGrid(G),
	allAvailableMoves(G,1,L),
	L = [M1|_],
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
testJasper([],[],J,MJ,0,R,LR,Capture):-
	!,
	statistics(runtime,[Depart,_]),
	initialGrid(G),
	simuUntilTimeout(Depart, [0,0,0,0,0,0,[],[]], G, 0, J,MJ, LR),
	getThroughs(LR, Throughs),
	getWinP1(LR,WinP1),
	getDraw(LR,Draw),
	display(Depart, 0),
	getMoveList(LR,L),
	display(Depart, 0),
	getMaxWinRate(L,P),
	getMove(P,TR),
	isCapture(TR, J, G, Capture),
  correct(TR,R).
testJasper(MoveHistory,MT,J,MJ,T,R,LR,Capture):-
	statistics(runtime,[Depart,_]),
	initialGrid(TG),
	startPlayer(J,T,FJ),
	reApplyMoves(MoveHistory, FJ, TG, G),
	last(MoveHistory,LastMove),
	correct(LastMove, CorrectedLastMove),
	getCorrectList(CorrectedLastMove, MT, M),
	simuUntilTimeout(Depart,M,G,T,J,MJ,LR),
	display(Depart, 0),
	getThroughs(LR, Throughs),
	getMoveList(LR,L),
	getMaxWinRate(L,P),
	getMove(P,TR),
	isCapture(TR, J, G, Capture),
  correct(TR,R).

isCapture([_,T],J, G, 1):-
	Opp is -J,
	\+ validSuper(T,Opp,G).
isCapture(_,_,_,0).

getMaxWinRate([M],M):-!.
getMaxWinRate([M|L],R):-
	getMaxWinRate(L,T),
	getWinP1(M,WM),
	getThroughs(M,TM),
	getWinP1(T,WT),
	getThroughs(T,TT),
	getMaxMove(M,WM/TM,T,WT/TT,R).

startPlayer(T,J,FJ):-
	M is T mod 2,
	M \= 0,
	!,
	FJ is -J.

startPlayer(T,J,J).

reApplyMoves([],_,G,G).

reApplyMoves([[M,T]|MH],J,TG,G):-
	correct([M,T],[M2,T2]),
	actuallyMovePiece(M2,J,TG,T2,GR),
	Opp is -J,
	reApplyMoves(MH,Opp,GR,G).

correct([[[A,B],C],[D,E]],[[[A,F],C],[D,G]]):-
	F is 5-B,
	G is 5-E.
	
genFile(J,MJ):-
	initialGrid(G),
	statistics(runtime,[Depart,_]),
	simuUntilTimeout(Depart, [0,0,0,0,0,0,[],[]], G, 0, J, MJ, LR),
	write(LR).

testMat :-
	piece(Kodama, kodama),
	piece(Oni, oni),
	piece(SO, super_oni),
	piece(KP, koropokkuru),
	piece(KI, kirin),
	piece(KS, kodama_samourai),

	P1 = [
				[[0,2],Kodama],
				[[0,3],Oni],
				[[1,4],SO],
				[[3,0],KP],
				[[3,1],KI],
				[[3,3],Oni]
			 ],
	P2 = [
				[[0,0],KS],
				[[1,1],KS],
				[[1,2],KI],
				[[2,2],Kodama],
				[[2,5],KP],
				[[3,5],KI],
				[[4,5],Oni]
			 ],
	G = [P1,P2,[],[]],
	hasWin(G,J).

testTestJasper:-
	testJasper([],[],-1,-1,0,R,LR,C),
	MH = [R],
	testJasper(MH,LR,1,-1,1,R2,LR2,C2),
	write(R2),nl,
	write(C2),nl.