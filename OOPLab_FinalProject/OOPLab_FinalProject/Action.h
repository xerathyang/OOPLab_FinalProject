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
private:
	//type:
	//0: move wasd:0123 (firstpos/secondpos)
	//1: attack (atkmodify/range)
	//2: heal
	//3: shield
	int _type, _param1, _param2;
	bool _ranged;
	string _moveway;
};