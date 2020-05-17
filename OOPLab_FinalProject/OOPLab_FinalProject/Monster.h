#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "Action.h"

using namespace std;

class Monster {
public:
	Monster();
	Monster(ifstream&);

private:
	string _name;
	int _nlife, _natk, _nrange, _elife, _eatk, _erange;
	vector<MonsterSkill> _monstercard;
};

class MonsterData {
public:
	MonsterData();
	MonsterData(string);
private:
	int _monsterTypecount;
	vector<Monster> _monsterlist;
};

class MonsterSkill {
public:
	MonsterSkill();
	MonsterSkill(ifstream&,int);
private:
	int _index, _dex;
	vector<Action> _action;
	bool _shuffle;
};