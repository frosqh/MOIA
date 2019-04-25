:-set_prolog_flag(toplevel_print_options,[max_depth(0)]).
:-['./movePiece.pl'].


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
	movePiece(P,1,G,R).