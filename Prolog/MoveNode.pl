%MoveNode : [Move, Throughs, WinP1, WinP2, Draw, ValueUCB1, KeyList, MoveList]

getC(C):-
	C is sqrt(2).
getD(D):-
	D is 1/3.

getMove([Move,_,_,_,_,_,_,_],Move).

incrThroughs([Move,Throughs,WinP1,WinP2,Draw,ValueUCB1,KeyList,MoveList]
						,[Move,ThroughR,WinP1,WinP2,Draw,ValueUCB1,KeyList,MoveList]):-
	ThroughR is Throughs+1.

getThroughs([_,Throughs,_,_,_,_,_,_],Throughs).

incrWinP1([Move,Throughs,WinP1,WinP2,Draw,ValueUCB1,KeyList,MoveList]
						,[Move,Throughs,WiRP1,WinP2,Draw,ValueUCB1,KeyList,MoveList]):-
	WiRP1 is WinP1+1.

getWinP1([_,_,WinP1,_,_,_,_,_],WinP1).

incrWinP2([Move,Throughs,WinP1,WinP2,Draw,ValueUCB1,KeyList,MoveList]
						,[Move,Throughs,WinP1,WiRP2,Draw,ValueUCB1,KeyList,MoveList]):-
	WiRP2 is WinP2+1.

getWinP2([_,_,_,WinP2,_,_,_,_],WinP2).

incrDraw([Move,Throughs,WinP1,WinP2,Draw,ValueUCB1,KeyList,MoveList]
						,[Move,Throughs,WinP1,WinP2,DraR,ValueUCB1,KeyList,MoveList]):-
	DraR is Draw+1.

getDraw([_,_,_,_,_,Draw,_,_,_],Draw).

updateValue([Move,Throughs,WinP1,WinP2,Draw,_,KeyList,MoveList]
						, ParentThroughs
						,[Move,Throughs,WinP1,WinP2,Draw,ValueUCBR,KeyList,MoveList]):-
	getC(C),
	getD(D),
	ValueUCBR is (WinP1/Throughs) + C*sqrt(log(ParentThroughs)/Throughs) + D*(Draw/Throughs).

getValue([_,_,_,_,_,ValueUCB1,_,_],ValueUCB1).

maxValue(MoveList,MaxMove):-
	getMoveList(MoveList,MoveListMoveList),
	getThroughs(MoveList, PT),
	maxValueBis(MoveListMoveList,PT,MaxMove).

maxValueBis([MoveNode],_, MoveNode).

maxValueBis([MoveNode | MoveList], PT, MaxMove):-
	maxValueBis(MoveList, PT, TempMaxMove),
	updateValue(MoveNode,PT,MoveNode2),
	updateValue(TempMaxMove, PT, TempMaxMove2),
	getValue(MoveNode2, Value1),
	getValue(TempMaxMove2, Value2),
	getMaxMove(MoveNode2,Value1,TempMaxMove2,Value2,MaxMove).

getMaxMove(Move1, Value1, _, Value2, Move1):-
	Value1 >= Value2.

getMaxMove(_, Value1, Move2, Value2, Move2):-
	Value1 < Value2.

getKeyList([_,_,_,_,_,_,KeyList,_],KeyList).

getMoveList([_,_,_,_,_,_,_,MoveList],MoveList).

setMoveList([Move,Throughs,WinP1,WinP2,Draw,ValueUCB1,KeyList,MoveList],
						MoveListBis,
						[Move,Throughs,WinP1,WinP2,Draw,ValueUCB1,KeyList,MoveListBis]).

addMove(Move1, [Move,Throughs,WinP1,WinP2,Draw,ValueUCB1,KeyList,MoveList]
						, [Move,Throughs,WinP1,WinP2,Draw,ValueUCB1,KeyList1,MoveListBis]):-
		append([Move1], KeyList, KeyList1),
		append([[Move1, 0, 0, 0, 0, 0, [], []]],MoveList,MoveListBis).
	
getCorrectList(Move, MoveList, CorrectMoveList):-
	getMoveList(MoveList, RealMoveList),
	getCorrectListBis(Move, RealMoveList, CorrectMoveList).

getCorrectListBis(Move, [MoveNode|_], MoveNode):-
	MoveNode = [Move|_],!.

getCorrectListBis(Move, [_|MoveList], CorrectMoveList):-
	getCorrectListBis(Move, MoveList, CorrectMoveList).

changeMoveList(Move, [MoveNode|MoveList], ToAddMoveList, ResMoveList) :-
	MoveNode = [Move,Throughs,WinP1,WinP2,Draw,ValueUCB1,KeyList,_],
	!,
	ResMoveList = [ToAddMoveList | MoveList].

changeMoveList(Move, [MoveNode|MoveList], ToAddMoveList, ResMoveList):-
	changeMoveList(Move,MoveList,ToAddMoveList,TMoveList),
	append([MoveNode],TMoveList, ResMoveList).
	
%Ici, remplacer la valeur de la moveList par celle en paramètre pour le bon move

updateValueWin(MoveList, 1, NewMoveList):-
	incrWinP1(MoveList, NewMoveList).

updateValueWin(MoveList, -1, NewMoveList):-
	incrWinP2(MoveList, NewMoveList).

updateValueWin(MoveList, 0, NewMoveList):-
	incrDraw(MoveList, NewMoveList).
%En fonction  du winner (ou draw), mettre à jour 
%la valeur correspondant, puis calculer la value UCB1




%--------------------------------------------------------------------------

:-begin_tests(throughsTest).
test('incrThroughs - negative',[true(Throughs == -1)]):-
	incrThroughs([0,-2,0,0,0,0,0,0],[0,Throughs,0,0,0,0,0,0]).
test('incrThroughs - zero',[true(Throughs == 1)]):-
	incrThroughs([0,0,0,0,0,0,0,0],[0,Throughs,0,0,0,0,0,0]).
test('incrThroughs - positive',[true(Throughs == 5)]):-
	incrThroughs([0,4,0,0,0,0,0,0],[0,Throughs,0,0,0,0,0,0]).
test('getThroughs - negative',[true(Throughs == -2)]):-
	getThroughs([0,-2,0,0,0,0,0,0],Throughs).
test('getThroughs - zero',[true(Throughs == 0)]):-
	getThroughs([0,0,0,0,0,0,0,0],Throughs).
test('getThroughs - positive',[true(Throughs == 5)]):-
	getThroughs([0,5,0,0,0,0,0,0],Throughs).
:-end_tests(throughsTest).

:-begin_tests(winP1Test).
test('incrWinP1 - negative',[true(WinP1 == -1)]):-
	incrThroughs([0,0,-2,0,0,0,0,0],[0,0,WinP1,0,0,0,0,0]).
test('incrWinP1 - zero',[true(WinP1 == 1)]):-
	incrThroughs([0,0,0,0,0,0,0,0],[0,0,WinP1,0,0,0,0,0]).
test('incrWinP1 - positive',[true(WinP1 == 5)]):-
	incrThroughs([0,0,4,0,0,0,0,0],[0,0,WinP1,0,0,0,0,0]).
test('getWinP1 - negative',[true(WinP1 == -2)]):-
	getThroughs([0,0,-2,0,0,0,0,0],WinP1).
test('getWinP1 - zero',[true(WinP1 == 0)]):-
	getThroughs([0,0,0,0,0,0,0,0],WinP1).
test('getWinP1 - positive',[true(WinP1 == 5)]):-
	getThroughs([0,0,5,0,0,0,0,0],WinP1).
:-end_tests(winP1Test).
