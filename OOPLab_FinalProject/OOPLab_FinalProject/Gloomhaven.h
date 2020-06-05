#pragma once

#include <iostream>
#include <string>
#include <vector>
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
	void setCharacterNum(int num);
	int getCharacterNum() { return characterNum; };
	void init();
	int characterNum;

private:
	string characterFile, monsterFile;
	CharacterData* cd1;
	MonsterData* md1;
	MapData* map1;
	vector<Object> objectlist;
};