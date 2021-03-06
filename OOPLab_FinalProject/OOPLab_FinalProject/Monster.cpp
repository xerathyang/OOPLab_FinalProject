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
	string count, comp;
	stringstream ss;
	ss.str("");
	ss.clear();
	MonsterSkill *cache;
	getline(fs, count);
	ss << count;
	ss >> _name >> _nlife >> _natk >> _nrange >> _elife >> _eatk >> _erange;
	for (int i = 0; i < 6; i++) {
		cache = new MonsterSkill(fs, _nrange);
		_monstercard.push_back(*cache);
	}
}

int Monster::getdex(int index) {
	return this->_monstercard[index]._dex;
}

vector<Action>& Monster::getskill(int index) {
	return this->_monstercard[index]._action;
}


MonsterData::MonsterData() {
	_monsterlist.clear();
	_monsterTypecount = 0;
}

MonsterData::MonsterData(string MonsterPath) {
	ifstream fs;
	string count;
	stringstream ss;
	ss.str("");
	ss.clear();
	Monster* cache;
	fs.open(MonsterPath);
	getline(fs, count);
	ss << count;
	ss >> _monsterTypecount;
	for (int i = 0; i < _monsterTypecount; i++) {
		cache = new Monster(fs);
		_monsterlist.push_back(*cache);
	}
	fs.close();
}

MonsterSkill::MonsterSkill() {
	_index = 0;
	_dex = 99;
	_action.clear();
	_shuffle = false;
}

MonsterSkill::MonsterSkill(ifstream& fs, int range) {
	stringstream ss;
	string cache, skillline, name;
	Action* tmp = new Action();
	int movpos[2];
	int param1, param2;
	
	ss.str("");
	ss.clear();

	_shuffle = false;

	getline(fs, skillline);
	ss << skillline;
	ss >> name >> _index >> _dex;
	while (ss >> cache) {
		movpos[0] = 0;
		movpos[1] = 0;

		if (cache == "d") {
			_shuffle = false;
			break;
		}
		else if (cache == "r") {
			_shuffle = true;
			break;
		}
		else if (cache == "move") {
			ss >> cache;
			tmp = new Action(0, cache);
		}
		else if (cache == "attack") {
			ss >> param1;
			if (range > 0) {
				ss >> cache;
				ss >> param2;
				tmp = new Action(1, param1, param2, true);
			}
			else {
				tmp = new Action(1, param1, 0, false);
			}
		}
		else if (cache == "heal") {
			ss >> param1;
			tmp = new Action(2, param1, 0, false);
		}
		else if (cache == "shield") {
			ss >> param1;
			tmp = new Action(3, param1, 0, false);
		}
		_action.push_back(*tmp);
	}
}

Monster& MonsterData::find(string name) {
	Monster* ret = new Monster();
	for (int i = 0; i < _monsterTypecount; i++) {
		if (_monsterlist[i]._name == name) {
			ret = &_monsterlist[i];
		}
	}
	return *ret;
}