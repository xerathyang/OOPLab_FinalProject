#include "Monster.h"

using namespace std;

Monster::Monster() {
	_name = "";
	_nlife = 0;
	_natk = 0;
	_nrange = 0;
	_elife = 0;
	_eatk = 0;
	_erange = 0;
}

Monster::Monster(string name, int nlife, int natk, int nrange, int elife, int eatk, int erange) {
	_name = name;
	_nlife = nlife;
	_natk = natk;
	_nrange = nrange;
	_elife = elife;
	_eatk = eatk;
	_erange = erange;
}