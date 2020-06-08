#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "Action.h"

using namespace std;

class CharacterSkill {
public:
	CharacterSkill();
	CharacterSkill(ifstream&, int index);

	friend class Character;
private:
	int _index, _dex;
	vector<vector<Action>> _action;
};

class Character {
public:
	Character();
	Character(ifstream&);
	int getdex(int);

	vector<vector<Action>>& getskill(int);

	friend class Object;
	friend class CharacterData;
private:
	string _name;
	int _life, _startcard, _availablecard;
	vector<CharacterSkill> _charactercard;
};

class CharacterData {
public:
	CharacterData();
	CharacterData(string);
	Character& find(string);
private:
	int _charTypecount;
	vector<Character> _charlist;
};