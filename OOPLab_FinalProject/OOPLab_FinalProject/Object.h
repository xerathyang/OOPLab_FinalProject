#pragma once
#include "Character.h"
#include "Monster.h"
#include "Map.h"
#include "Action.h"
#include <string>
#include <vector>

class Object {
public:
	Object();
	Object& operator=(Character);
	Object& operator=(Monster);
private:
	bool _ismonster;
	string name;
	int _life, _atk, _def;
	vector<int> _cardindex;
};