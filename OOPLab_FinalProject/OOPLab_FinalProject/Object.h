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
	Point2d& getpos();

	friend class Gloomhaven;
private:
	bool _ismonster,_isactive;
	string _name;
	int _life, _atk, _def, _range, _startcardnum, _avaliablecard;
	Point2d _pos;
	set<int> _cardindex;
};