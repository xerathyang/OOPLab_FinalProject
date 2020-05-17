#pragma once

//1123
class Action {
public:
	Action();
	Action(int, int, int);
private:
	//type:
	//0: move wasd:0123 (firstpos/secondpos)
	//1: attack (atkmodify/range)
	//2: heal
	//3: shield
	int _type, _param1, _param2;
};