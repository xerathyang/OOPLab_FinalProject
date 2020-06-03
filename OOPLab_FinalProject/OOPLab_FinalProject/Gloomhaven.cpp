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

void init() {

}