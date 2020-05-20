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
	CharacterSkill(ifstream&);
private:
	int _index, _dex;
	vector<Action> _action;
};

class Character {
public:
	Character();
	Character(ifstream&);
private:
	string _name;
	int _life, _startcard;
};

class CharacterData {
public:
	CharacterData();
	CharacterData(string);
private:
	int _charTypecount;
	vector<Character> _charlist;
};