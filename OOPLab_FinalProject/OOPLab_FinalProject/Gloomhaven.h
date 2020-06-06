#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <time.h>
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
	void preparephrase();
	void printMap(int);
	void printMap(int, Point2d&);
	bool isoccupied(Point2d&);
	bool cardcheck(Object&, int, int);

	int characterNum;

private:
	int DEBUG_MODE;
	string characterFile, monsterFile;
	CharacterData* cd1;
	MonsterData* md1;
	MapData* map1;
	vector<Object> charlist;
	vector<Object> monsterlist;
	vector<vector<char>> visiblearea;
	set<string> monstertype;
};