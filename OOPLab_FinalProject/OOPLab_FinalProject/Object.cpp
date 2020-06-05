#include "Object.h"


Object::Object() {
	_ismonster = false;
	_name = "";
	_life = 0;
	_atk = 0;
	_def = 0;
	_range = 0;
	_startcardnum = 0;
}

bool Object::spawn(Character& tar) {
	if (tar._name == "")
		return true;
	_startcardnum = tar._startcard;
	_name = tar._name;
	_life = tar._life;
	
	return false;
}

bool Object::spawn(Monster& tar,bool iselite) {
	if (tar._name == "")
		return true;
	_ismonster = true;
	_name = tar._name;
	_startcardnum = 6;
	if (iselite) {
		_life = tar._elife;
		_atk = tar._eatk;
		_range = tar._erange;
	}
	else {
		_life = tar._nlife;
		_atk = tar._natk;
		_range = tar._nrange;
	}
	_cardindex = set<int>{ 0,1,2,3,4,5 };
	return false;
}