%----------------------------------
%				Units Tests util.pl
%----------------------------------
:-begin_tests(coorToStringTest).
test('coorToString - 0,0 -> 1,0', [true(S == "A6B6")]):-
	coorToString([0,0],[1,0],S).
test('coorToString - 4,5 -> 4,4', [true(S == "E1E2")]):-
	coorToString([4,5],[4,4],S).
:-end_tests(coorToStringTest).

%----------------------------------
%				Units Tests MoveNode.pl
%----------------------------------

:-begin_tests(moveTest).
test('getMove - list', [true(Move == [[[2,2],1],[2,3]])]):-
	getMove([[[[2,2],1],[2,3]], 42, 12, 10, 20, 0, [], []],Move).
test('getMove - value', [true(Move == 0)]):-
	getMove([0,0,0,0,0,0,0,0],Move).
:-end_tests(moveTest).

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
	incrWinP1([0,0,-2,0,0,0,0,0],[0,0,WinP1,0,0,0,0,0]).
test('incrWinP1 - zero',[true(WinP1 == 1)]):-
	incrWinP1([0,0,0,0,0,0,0,0],[0,0,WinP1,0,0,0,0,0]).
test('incrWinP1 - positive',[true(WinP1 == 5)]):-
	incrWinP1([0,0,4,0,0,0,0,0],[0,0,WinP1,0,0,0,0,0]).
test('getWinP1 - negative',[true(WinP1 == -2)]):-
	getWinP1([0,0,-2,0,0,0,0,0],WinP1).
test('getWinP1 - zero',[true(WinP1 == 0)]):-
	getWinP1([0,0,0,0,0,0,0,0],WinP1).
test('getWinP1 - positive',[true(WinP1 == 5)]):-
	getWinP1([0,0,5,0,0,0,0,0],WinP1).
:-end_tests(winP1Test).

