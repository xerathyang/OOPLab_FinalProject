#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "Action.h"

using namespace std;

class MonsterSkill {
public:
	MonsterSkill();
	MonsterSkill(ifstream&,int);

	friend class Monster;
private:
	int _index, _dex;
	vector<Action> _action;
	bool _shuffle;
};

class Monster {
public:
	Monster();
	Monster(ifstream&);
	int getdex(int);
	int getlife(bool elite) {
		if (elite)
			return _elife;
		else
			return _nlife;
	}
	vector<Action>& getskill(int);
	bool needshuffle(int index) { return _monstercard[index]._shuffle; };

	friend class Object;
	friend class MonsterData;
private:
	string _name;
	int _nlife, _natk, _nrange, _elife, _eatk, _erange;
	vector<MonsterSkill> _monstercard;
};

class MonsterData {
public:
	MonsterData();
	MonsterData(string);
	Monster& find(string);
private:
	int _monsterTypecount;
	vector<Monster> _monsterlist;
};

