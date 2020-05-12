#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "Action.h"

using namespace std;

class Monster {
public:
	Monster();
	Monster(string name, int nlife, int natk, int nrange, int elife, int eatk, int erange);
	void SetMonster();
private:
	string _name;
	int _nlife, _natk, _nrange, _elife, _eatk, _erange;
	vector<MonsterSkill> _monstercard;
};

class MonsterData {
public:
	MonsterData();
	void SetMonsterData();
private:
	int _monsterTypecount;
	vector<Monster> _monsterlist;
};

class MonsterSkill {
public:
	MonsterSkill();
	void SetMonsterSkill();
private:
	int _index, _dex;
	vector<Action> _action;
	bool _shuffle;
};