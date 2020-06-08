#pragma once
#include <string>
#include <iostream>

using namespace std;

class Action {
public:
	Action();
	Action(int, string);
	Action(int, int, int, bool);
	void printAction();
	int gettype() { return _type; };
	int getparam1() { return _param1; };
	int getparam2() { return _param2; };
	string getmoveway() { return _moveway; };
	
private:
	//type:
	//0: move
	//1: attack (atkmodify/range)
	//2: heal
	//3: shield
	int _type, _param1, _param2;
	bool _ranged;
	string _moveway;
};