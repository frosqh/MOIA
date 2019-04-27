%MoveNode : [Move, Throughs, WinP1, WinP2, Draw, ValueUCB1, KeyList, MoveList]

getC(C):-
	C is sqrt(2).

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
	ValueUCBR is (WinP1/Throughs) + C*sqrt(log(ParentThroughs)/Throughs).

getValue([_,_,_,_,_,ValueUCB1,_,_],ValueUCB1).

maxValue(MoveList, MaxMove):-
	getMoveList(MoveList,MoveListMoveList),
	maxValueBis(MoveListMoveList,MaxMove).

maxValueBis([MoveNode], MaxMove):-
	getValue(MoveNode, MaxMove).

maxValueBis([MoveNode | MoveList], MaxMove):-
	maxValueBis(MoveList, TempMaxMove),
	getValue(MoveNode, Value1),
	getValue(MoveNode, Value2),
	getMaxMove(MoveNode,Value1,TempMaxMove,Value2,MaxMove).

getMaxMove(Move1, Value1, _, Value2, ResMove):-
	Value1 >= Value2,
	ResMove = Move1.

getMaxMove(_, Value1, Move2, Value2, ResMove):-
	Value1 < Value2,
	ResMove = Move2.

getKeyList([_,_,_,_,_,_,KeyList,_],KeyList).

getMoveList([_,_,_,_,_,_,_,MoveList],MoveList).

addMove(Move1, [Move,Throughs,WinP1,WinP2,Draw,ValueUCB1,KeyList,MoveList]
						, [Move,Throughs,WinP1,WinP2,Draw,ValueUCB1,KeyList,MoveListBis]):-
		append([[Move1, 0, 0, 0, 0, 0, [], []]],MoveList,MoveListBis).
	


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