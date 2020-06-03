#include "Gloomhaven.h"

Gloomhaven::Gloomhaven() {
	characterFile = "";
	monsterFile = "";
	characterNum = 0;
}

void Gloomhaven::setFilePath(string cFile, string mFile) {
	characterFile = cFile;
	monsterFile = mFile;
}

void Gloomhaven::setCharacterNum(int num) {
	characterNum = num;
}

void Gloomhaven::init() {
	string mapname;
	cd1 = new CharacterData(characterFile);
	md1 = new MonsterData(monsterFile);
	map1 = new MapData();
	cout << endl;
	cout << "Enter the map name: ";
	cin >> mapname;
	map1->SetMap(mapname);
}