#include "Object.h"


Object::Object() {
	_ismonster = false;
	_isactive = false;
	_hasmoved = false;
	_iselite = false;
	_isdead = false;
	_name = "";
	_life = 0;
	_atk = 0;
	_def = 0;
	_dex = 0;
	_range = 0;
	_shield = 0;
	_startcardnum = 0;
	_avaliablecard = 0;
	_mapid = ' ';

	_card1 = -1;
	_card2 = -1;
}

bool Object::spawn(Character& tar) {
	if (tar._name == "")
		return true;
	_startcardnum = tar._startcard;
	_avaliablecard = tar._availablecard;
	_name = tar._name;
	_maximumlife = tar._life;
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
		_iselite = true;
		_maximumlife = tar._elife;
		_life = tar._elife;
		_atk = tar._eatk;
		_range = tar._erange;
	}
	else {
		_maximumlife = tar._nlife;
		_life = tar._nlife;
		_atk = tar._natk;
		_range = tar._nrange;
	}
	_cardindex = set<int>{ 0,1,2,3,4,5 };
	return false;
}

Point2d& Object::getpos() {
	return _pos;
}

void Object::removecard(int card1, int card2) {
	set<int>::iterator ster = _cardindex.begin();
	while (ster != _cardindex.end()) {
		if (*ster == card1) {
			_cardindex.erase(ster);
			_discardindex.insert(card1);
			break;
		}
		ster++;
	}
	ster = _cardindex.begin();
	while (ster != _cardindex.end()) {
		if (*ster == card2) {
			_cardindex.erase(ster);
			_discardindex.insert(card2);
			break;
		}
		ster++;
	}

}

bool Object::shuffle(int removecard) {
	set<int>::iterator ster = _discardindex.begin();
	auto search = _discardindex.find(removecard);
	if (search == _discardindex.end()) {
		return false;
	}
	
	while (ster != _discardindex.end()) {
		if (*ster == removecard) {
			_discardindex.erase(ster);
			break;
		}
		ster++;
	}
	ster = _discardindex.begin();
	while (ster != _discardindex.end()) {
		_cardindex.insert(*ster);
		ster++;
	}
	_discardindex.clear();
	return true;
}

void Object::regen(int value) {
	if (_life + value > _maximumlife)
		_life = _maximumlife;
	else
		_life += value;
}

void Object::damage(int value) {
	if (value - _shield > 0)
		_life -= (value-_shield);
}