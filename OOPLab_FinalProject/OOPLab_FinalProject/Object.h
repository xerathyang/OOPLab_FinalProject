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
	int getdex() { return _dex; };
	string getname() { return _name; };
	void removecard(int, int);
	void shuffle(int);
	void regen(int);
	void damage(int);

	friend class Gloomhaven;
private:
	bool _ismonster, _isactive, _hasmoved, _iselite, _isdead;
	string _name;
	int _life, _atk, _def, _dex, _range, _shield, _startcardnum, _avaliablecard, _maximumlife;
	int _card1, _card2;
	char _mapid;
	Point2d _pos;
	set<int> _cardindex, _discardindex;
	
};