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
#include <algorithm>
#include <utility>
#include <time.h>


using namespace std;

class Gloomhaven {
public:
	Gloomhaven();
	void setFilePath(string cFile,string mFile);
	int getCharacterNum() { return characterNum; };
	void init();
	void preparephrase();
	void actionphrase();

	bool isend();
	void checkdoor();
	void roundreset();
	void printMap(int);
	void printMap(int, Point2d&);
	bool isoccupied(Point2d&);
	bool isfriend(Point2d&, bool);
	bool isvalidpos(Point2d&);
	bool cardcheck(Object&, int, int);
	Object& findbyId(char);

	void HandleAction(Object&, vector<Action>&);
	bool Objectdistance(Object&, Object&, int);

	//
	void MonsterFindAndAttack(Object &mon, int atk, int range);
	//bool CompareForMFA(pair<Object&, int> a, pair<Object&, int> b);
	bool FindBarrier(Point2d p1, Point2d p2);
	//
	int characterNum;

private:
	int DEBUG_MODE;
	unsigned round;
	string characterFile, monsterFile;
	CharacterData* cd1;
	MonsterData* md1;
	MapData* map1;
	vector<Object> charlist;
	vector<Object> monsterlist;
	vector<vector<char>> visiblearea;
	set<string> monstertype;
};