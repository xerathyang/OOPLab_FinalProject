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

//in progress
Monster::Monster(ifstream& fs) {
	string comp;
	MonsterSkill cache;
	fs >> _name >> _nlife >> _natk >> _nrange >> _elife >> _eatk >> _erange;
	for (int i = 0; i < 6; i++) {
		fs >> comp;
		if (comp != _name) {
			cout << "monsterskill error" << endl;
		}
		cache = new MonsterSkill(fs);
		_monstercard.push_back(cache);
	}
}

MonsterData::MonsterData() {
	_monsterlist.clear();
	_monsterTypecount = 0;
}

MonsterData::MonsterData(string MonsterPath) {
	ifstream fs;
	Monster cache;
	fs.open(MonsterPath);
	fs >> _monsterTypecount;
	for (int i = 0; i < _monsterTypecount; i++) {
		cache = new Monster(fs);
		_monsterlist.push_back(cache);
	}
	fs.close();
}

MonsterSkill::MonsterSkill() {
	_index = 0;
	_dex = 99;
	_action.clear();
	_shuffle = false;
}
//in progress
MonsterSkill::MonsterSkill(ifstream& fs) {
	stringstream ss;
	string skillline;

	ss.str("");
	ss.clear();
	getline(fs, skillline);
	ss << skillline;
	ss >> _index >> _dex;
}