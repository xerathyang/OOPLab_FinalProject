#pragma once
#include <string>
#include <fstream>
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