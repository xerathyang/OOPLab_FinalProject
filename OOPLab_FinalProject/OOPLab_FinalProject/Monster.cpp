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
	MonsterSkill *cache;
	fs >> _name >> _nlife >> _natk >> _nrange >> _elife >> _eatk >> _erange;
	for (int i = 0; i < 6; i++) {
		fs >> comp;
		if (comp != _name) {
			cout << "monsterskill error" << endl;
		}
		cache = new MonsterSkill(fs, _nrange);
		_monstercard.push_back(*cache);
	}
}

MonsterData::MonsterData() {
	_monsterlist.clear();
	_monsterTypecount = 0;
}

MonsterData::MonsterData(string MonsterPath) {
	ifstream fs;
	Monster *cache;
	fs.open(MonsterPath);
	fs >> _monsterTypecount;
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
	string cache;
	string skillline;
	Action* tmp = new Action();
	int movpos[2];
	int param1, param2;
	
	_shuffle = false;

	movpos[0] = 0;
	movpos[1] = 0;

	ss.str("");
	ss.clear();
	getline(fs, skillline);
	ss << skillline;
	ss >> _index >> _dex;
	while (ss >> cache) {
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
			for (int j = 0; j < 2; j++) {
				switch (cache[j])
				{
				case 'w':
					movpos[j] = 0;
					break;
				case 'a':
					movpos[j] = 1;
					break;
				case 's':
					movpos[j] = 2;
					break;
				case 'd':
					movpos[j] = 3;
					break;
				default:
					break;
				}
			}
			tmp = new Action(0, movpos[0], movpos[1]);
		}
		else if (cache == "attack") {
			ss >> param1;
			if (range > 0) {
				ss >> cache;
				ss >> param2;
				tmp = new Action(1, param1, param2);
			}
			else {
				tmp = new Action(1, param1, 0);
			}
		}
		else if (cache == "heal") {
			ss >> param1;
			tmp = new Action(2, param1, 0);
		}
		else if (cache == "shield") {
			ss >> param1;
			tmp = new Action(3, param1, 0);
		}
		_action.push_back(*tmp);
	}
}