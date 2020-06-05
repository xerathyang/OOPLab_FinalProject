#pragma once
#include "Character.h"
#include "Monster.h"
#include "Map.h"
#include "Action.h"
#include "Gloomhaven.h"
#include <string>
#include <set>

class Object {
public:
	Object();
	bool spawn(Character&);
	bool spawn(Monster&,bool);

	friend class Gloomhaven;
private:
	bool _ismonster;
	string _name;
	int _life, _atk, _def, _range, _startcardnum;
	set<int> _cardindex;
};