%MoveNode : [Move, Throughs, WinP1, WinP2, Draw, ValueUCB1, KeyList, MoveList]

%Fichier de définition de la structure MoveNode sous la forme :
	%[[Move, Throughs, WinP1, WinP2, Draw, ValueUCB1, KeyList, MoveList]*] avec :
		%Move de la forme [[X,Y],[X2,Y2]] symbolisant un coup traité
		%Throughs = Le nombre de simulation passant par ce noeud
		%WinP1 = Le nombre de victoires de P1 après être passé par ce noeud
		%WinP2 = Le nombre de victoires de P2 après être passé par ce noeud
		%Draw  = Le nombre d'égalité après être passé par ce noeud
		%ValueUCB1 = La valeur UCB1 du noeud, afin d'optimiser l'expansion des rollouts
		%KeyList = La liste des moves fils déjà traités (pour accelérer le traitement UCT)
		%MoveList = L'arbre des rollouts à partir de ce  noeud

%:-getC/1
%Fournit la constante utilisée lors du calcul de la valeur UCB1
getC(C):-
	C is sqrt(2).

%:-getD/1
%Fournit la constante déterminant le coefficient d'importance des draws
getD(D):-
	D is 1/3.

%:-getMove/2
%Récupère le champ move d'une structure MoveNode
%getMove(MoveNode, Move) : 
				%MoveNode = Noeud de l'arbre de la même forme que ci-dessus
				%Move = Move du noeud MoveNode [O]
getMove([Move,_,_,_,_,_,_,_],Move).

%:-incrThroughs/2
%Renvoie un nouveau ModeNode avec le nombre de parcours incrémenté
%incrThroughs(MoveNode, MoveNode1) :
				%MoveNode = Noeud d'origine
				%MoveNode1 = Noeud incrémenté [O]
incrThroughs([Move,Throughs,WinP1,WinP2,Draw,ValueUCB1,KeyList,MoveList]
						,[Move,ThroughR,WinP1,WinP2,Draw,ValueUCB1,KeyList,MoveList]):-
	ThroughR is Throughs+1.

%:-getThroughs/2
%Récupère le champ Throughs d'une structure MoveNode
%getThroughs(MoveNode, Throughs) :
				%MoveNode = Noeud de l'arbre de la même forme que ci-dessus
				%Throughs = Nombre de parcours de ce noeud [O]
getThroughs([_,Throughs,_,_,_,_,_,_],Throughs).

%:-incrWinP1/2
%Renvoie un nouveau MoveNode avec le nombre de victoires du joueur incrémenté
%incrWinP1(MoveNode, MoveNode1) : 
				%MoveNode = Noeud d'origine
				%MoveNode1 = Noeud incrémenté [O]
incrWinP1([Move,Throughs,WinP1,WinP2,Draw,ValueUCB1,KeyList,MoveList]
						,[Move,Throughs,WiRP1,WinP2,Draw,ValueUCB1,KeyList,MoveList]):-
	WiRP1 is WinP1+1.

%:-getWinP1/2
%Récupère le champ WinP1 d'une structure MoveNode
%getWinP1(MoveNode, WinP1) :
				%MoveNode = Noeud d'origine
				%WinP1 = Nombre de parcours gagnant de ce noeud [O]
getWinP1([_,_,WinP1,_,_,_,_,_],WinP1).

%:-incrWinP2/2
%Renvoie un nouveau MoveNode avec le nombre de victoires de l'adversaire incrémenté
%incrWinP2(MoveNode, MoveNode1) : 
				%MoveNode = Noeud d'origine
				%MoveNode1 = Noeud incrémenté [O]
incrWinP2([Move,Throughs,WinP1,WinP2,Draw,ValueUCB1,KeyList,MoveList]
						,[Move,Throughs,WinP1,WiRP2,Draw,ValueUCB1,KeyList,MoveList]):-
	WiRP2 is WinP2+1.

%:-getWinP2/2
%Récupère le champ WinP2 d'une structure MoveNode
%getWinP1(MoveNode, WinP2) :
				%MoveNode = Noeud d'origine
				%WinP2 = Nombre de parcours perdant de ce noeud [O]
getWinP2([_,_,_,WinP2,_,_,_,_],WinP2).

%:-incrDraw/2
%Renvoie un nouveau MoveNode avec le nombre d'égalités incrémenté
%incrDraw(MoveNode, MoveNode1) : 
				%MoveNode = Noeud d'origine
				%MoveNode1 = Noeud incrémenté [O]
incrDraw([Move,Throughs,WinP1,WinP2,Draw,ValueUCB1,KeyList,MoveList]
						,[Move,Throughs,WinP1,WinP2,DraR,ValueUCB1,KeyList,MoveList]):-
	DraR is Draw+1.

%:-getDraw/2
%Récupère le champ Draw d'une structure MoveNode
%getWinP1(MoveNode, Draw) :
				%MoveNode = Noeud d'origine
				%Draw = Nombre de parcours avec égalité de ce noeud [O]
getDraw([_,_,_,_,Draw,_,_,_],Draw).


%updateValue/3
%Renvoie un nouveau MoveNode dont la valeur UCB a été mise à jour
%updateValue(MoveNode, ParentThroughs, MoveNode1) :
				%MoveNode = Noeud d'origine
				%ParentThroughs = Nombre de parcours passant par le noeud parent
				%MoveNode1 = Noeud actualisé
updateValue([Move,Throughs,WinP1,WinP2,Draw,_,KeyList,MoveList]
						, ParentThroughs
						,[Move,Throughs,WinP1,WinP2,Draw,ValueUCBR,KeyList,MoveList]):-
	getC(C),
	getD(D),
	ValueUCBR is (WinP1/Throughs) + C*sqrt(log(ParentThroughs)/Throughs) + D*(Draw/Throughs).


getValue([_,_,_,_,_,ValueUCB1,_,_],ValueUCB1).

maxValue(MoveList, MJ, MaxMove):-
	getMoveList(MoveList,MoveListMoveList),
	getThroughs(MoveList, PT),
	maxValueBis(MoveListMoveList,MJ, PT,MaxMove).

maxValueBis([MoveNode],_,_, MoveNode).

maxValueBis([MoveNode | MoveList],MJ, PT, MaxMove):-
	maxValueBis(MoveList,MJ, PT, TempMaxMove),
	updateValue(MoveNode,PT,MoveNode2),
	updateValue(TempMaxMove,PT, TempMaxMove2),
	getValue(MoveNode2, Value1),
	getValue(TempMaxMove2, Value2),
	getMaxMove(MoveNode2,Value1,TempMaxMove2,Value2,MaxMove).

getMaxMove(Move1, Value1, _, Value2, Move1):-
	Value1 >= Value2.

getMaxMove(_, Value1, Move2, Value2, Move2):-
	Value1 < Value2.

getKeyList([_,_,_,_,_,_,KeyList,_],KeyList).

getMoveList([_,_,_,_,_,_,_,MoveList],MoveList).

setMoveList([Move,Throughs,WinP1,WinP2,Draw,ValueUCB1,KeyList,_],
						MoveListBis,
						[Move,Throughs,WinP1,WinP2,Draw,ValueUCB1,KeyList,MoveListBis]).

addMove(Move1, [Move,Throughs,WinP1,WinP2,Draw,ValueUCB1,KeyList,MoveList]
						, [Move,Throughs,WinP1,WinP2,Draw,ValueUCB1,KeyList1,MoveListBis]):-
		append([Move1], KeyList, KeyList1),
		append([[Move1, 0, 0, 0, 0, 0, [], []]],MoveList,MoveListBis).
	
getCorrectList(_,[],[0, 0, 0, 0, 0, 0, [], []]):-!.
getCorrectList(Move, MoveList, CorrectMoveList):-
	getMoveList(MoveList, RealMoveList),
	getCorrectListBis(Move, RealMoveList, CorrectMoveList).

getCorrectListBis(Move, [], [Move, 0, 0, 0, 0, 0, [], []]):-!.

getCorrectListBis(Move, [MoveNode|_], MoveNode):-
	MoveNode = [Move|_],!.

getCorrectListBis(Move, [_|MoveList], CorrectMoveList):-
	getCorrectListBis(Move, MoveList, CorrectMoveList).

changeMoveList(Move, [MoveNode|MoveList], ToAddMoveList, ResMoveList) :-
	MoveNode = [Move|_],
	!,
	ResMoveList = [ToAddMoveList | MoveList].

changeMoveList(Move, [MoveNode|MoveList], ToAddMoveList, ResMoveList):-
	changeMoveList(Move,MoveList,ToAddMoveList,TMoveList),
	append([MoveNode],TMoveList, ResMoveList).
	
%Ici, remplacer la valeur de la moveList par celle en paramètre pour le bon move

updateValueWin(MoveList, MJ, MJ, NewMoveList):-
	incrWinP1(MoveList, NewMoveList).

updateValueWin(MoveList, J, MJ, NewMoveList):-
	J is -MJ,
	incrWinP2(MoveList, NewMoveList).

updateValueWin(MoveList, 0, _, NewMoveList):-
	incrDraw(MoveList, NewMoveList).
%En fonction  du winner (ou draw), mettre à jour 
%la valeur correspondant, puis calculer la value UCB1




