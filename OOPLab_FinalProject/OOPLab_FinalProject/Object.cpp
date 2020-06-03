#include "Object.h"

Object::Object() {
	_ismonster = false;
	_name = "";
	_life = 0;
	_atk = 0;
	_def = 0;
}

Object& Object::operator=(Character& tar) {
	Object* ret = new Object();
	ret->_name = tar._name;
	ret->_life = tar._life;
	
	return *ret;
}

Object& Object::operator=(Monster& tar) {
	Object* ret = new Object();
	ret->_ismonster = true;
	
	
}