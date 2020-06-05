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
	Gloomhaven::characterNum = num;
}

void Gloomhaven::init() {
	string mapname, charName, cache;
	int indexcache;
	bool flag;
	stringstream ss;
	cd1 = new CharacterData(characterFile);
	md1 = new MonsterData(monsterFile);
	map1 = new MapData();

	cout << "Enter Character Number: ";
	getline(cin, cache);
	ss << cache;
	ss >> characterNum;
	
	for (int i = 0; i < characterNum; i++) {
		Object* tmp = new Object();
		flag = true;
		ss.str("");
		ss.clear();
		getline(cin, cache);
		ss << cache;
		ss >> charName;
		if (tmp->spawn(cd1->find(charName))) {
			i--;
			cout << "No character's name fits this name." << endl;
			continue;
		}
		for (int j = 0; j < tmp->_startcardnum; j++) {
			ss >> indexcache;
			auto search = tmp->_cardindex.find(indexcache);
			if (search == tmp->_cardindex.end())
				tmp->_cardindex.insert(indexcache);
			else {
				i--;
				flag = false;
				cout << "You cannot bring duplicate cards, please choose again." << endl;
				break;
			}
		}
		if (!flag)
			continue;
	}

	cout << endl;
	cout << "Enter the map name: ";
	cin >> mapname;
	map1->SetMap(mapname);
}