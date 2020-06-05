#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include "Monster.h"
#include "Character.h"
#include "Map.h"
#include "Object.h"

using namespace std;

class Gloomhaven {
public:
	Gloomhaven();
	void setFilePath(string cFile,string mFile);
	int getCharacterNum() { return characterNum; };
	void init();
	void printMap();
	int characterNum;

private:
	string characterFile, monsterFile;
	CharacterData* cd1;
	MonsterData* md1;
	MapData* map1;
	vector<Object> charlist;
	vector<Object> monsterlist;
	set<string> monstertype;
};