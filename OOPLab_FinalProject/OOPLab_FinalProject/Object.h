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
	void operator=(Character);
	void operator=(Monster);
private:
	bool _ismonster;
	int _life;
};