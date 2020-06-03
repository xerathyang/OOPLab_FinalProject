#pragma once

#include <fstream>
#include <string>
#include <vector>
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
private:
	string characterFile, monsterFile;
	int characterNum;
};