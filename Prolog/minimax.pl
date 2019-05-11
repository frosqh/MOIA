%Bon, du coup, comment je veux travailler sur le minimax ?
%je pourrais simplement prendre l'ensemble des coups possibles, puis appeler là deuss, etc.
%MAIS, cela serait-il gênant pour un alpha-bêta ? Non pas vraiment
%Problème résolu, le minimax me paraît être vraiment TRES TRES peu de travail ^^'

maxDepth(2).

minimax(G,J,T,Depth,):-
	maxDepth(D),
	Depth > D,!.

minimax(G,J,T,Depth,):-
	T > 60.

minimax(G, J, T, Depth, ):-
	allAvailablePlays(G,J,Plays),
	mixList(G,J,T,Depth,Plays)

mixList(G,J,T,Depth,)
