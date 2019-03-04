%j = Joueur (1 en bas, -1 au dessus)
moveForward([X,Y],J,[X,Y2]) :- Y2 is Y+J.
moveBackward([X,Y],J,[X,Y2]) :- Y2 is Y-J.
moveRight([X,Y],J,[X2,Y]) :- X2 is X+J.
moveLeft([X,Y],J,[X2,Y]) :- X2 is X-J.
moveFR([X,Y],J,[X2,Y2]) :- X2 is X+J,Y2 is Y+J.
moveFL([X,Y],J,[X2,Y2]) :- X2 is X+J,Y2 is Y-J.
moveBR([X,Y],J,[X2,Y2]) :- X2 is X-J,Y2 is Y+J.
moveBL([X,Y],J,[X2,Y2]) :- X2 is X-J,Y2 is Y-J.

moveKd(A,J,R) :- moveForward(A,J,R).

moveS(A,J,R) :- moveForward(A,J,R).
moveS(A,J,R) :- moveBackward(A,J,R).
moveS(A,J,R) :- moveRight(A,J,R).
moveS(A,J,R) :- moveLeft(A,J,R).
moveS(A,J,R) :- moveFR(A,J,R).
moveS(A,J,R) :- moveFL(A,J,R).

moveKdS(A,J,R) :- moveS(A,J,R).
moveKr(A,J,R) :- moveS(A,J,R).
moveSO(A,J,R) :- moveS(A,J,R).

moveKk(A,J,R) :- moveS(A,J,R).
moveKk(A,J,R) :- moveBR(A,J,R).
moveKk(A,J,R) :- moveBL(A,J,R).

moveO(A,J,R) :- moveForward(A,J,R).
moveO(A,J,R) :- moveFR(A,J,R).
moveO(A,J,R) :- moveFL(A,J,R).
moveO(A,J,R) :- moveBR(A,J,R).
moveO(A,J,R) :- moveBL(A,J,R).

valid([X,Y]):-
	X >= 0,
	Y >= 0,
	5 >= X,
	5 >= Y.%TODO Rajouter la gestion des interdictions + deux yokai sur la même case

upgradeZone(1,[4,5]).
upgradeZone(-1,[0,1]).

upgrade([[X,Y],N],J,R):- %Si joueur 1, 4 & 5, si joueur -1, 0 & 1
	piece(A,kodama),
	piece(B,oni),
	member(N,[A,B]),
	upgradeZone(J,L), % L = [zone d'upgrade]
	member(Y,L),
	!,
	M is N+1,
	R = [[X,Y],M].

upgrade(A,_,A).

move([A,B],J,R):-
	piece(B,kodama),
	moveKd(A,J,R).
move([A,B],J,R):-
	piece(B,kodama_samourai),
	moveKdS(A,J,R).
move([A,B],J,R):-
	piece(B,kirin),
	moveKr(A,J,R).
move([A,B],J,R):-
	piece(B,koropokkuru),
	moveKk(A,J,R).
move([A,B],J,R):-
	piece(B,oni),
	moveO(A,J,R).
move([A,B],J,R):-
	piece(B,super_oni),
	moveSO(A,J,R).

piece(1,kodama).
piece(2,kodama_samourai).
piece(3,kirin).
piece(4,koropokkuru).
piece(5,oni).
piece(6,super_oni).

movePiece([A,N],J,R):-
	move([A,N],J,T),
	valid(T),
	upgrade([T,N],J,R).