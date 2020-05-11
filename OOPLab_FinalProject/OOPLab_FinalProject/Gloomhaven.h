#pragma once

#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Gloomhaven {
public:
	Gloomhaven();
	void setFilePath(string cFile,string mFile);
	void init();
private:
	string characterFile, monsterFile;
};