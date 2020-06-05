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

void Gloomhaven::init() {
	string mapname, charName, cache;
	int indexcache;
	bool flag;
	stringstream ss;
	cd1 = new CharacterData(characterFile);
	md1 = new MonsterData(monsterFile);
	map1 = new MapData();
	charlist.clear();
	monsterlist.clear();
	monstertype.clear();

	cout << "Enter Character Number: ";
	getline(cin, cache);
	ss << cache;
	ss >> characterNum;
	
	for (int i = 0; i < characterNum; i++) {
		cout << i + 1 <<". Enter character's name and the cards' index:" << endl;
		Object* tmp = new Object();
		flag = true;
		ss.str("");
		ss.clear();
		getline(cin, cache);
		ss << cache;
		ss >> charName;
		//if spawn failed
		if (tmp->spawn(cd1->find(charName))) {
			i--;
			cout << "No character's name fits this name." << endl;
			continue;
		}
		
		vector<int> cardcache;
		while (ss >> indexcache) {
			cardcache.push_back(indexcache);
		}

		//cards are less
		if (cardcache.size() < (unsigned)tmp->_startcardnum) {
			cout << "You must bring " << tmp->_startcardnum << " cards for this character." << endl;
			i--;
			continue;
		}
		//cards are more
		else if (cardcache.size() > (unsigned)tmp->_startcardnum) {
			cout << "You can only bring " << tmp->_startcardnum << " card for this character." << endl;
			i--;
			continue;
		}

		for (int j = 0; j < tmp->_startcardnum; j++) {
			if (cardcache[j] >= tmp->_avaliablecard) {
				i--;
				cout << "This character doesn't have \"" << cardcache[j] << "\"." << endl;
				break;
			}
			auto search = tmp->_cardindex.find(cardcache[j]);
			if (search == tmp->_cardindex.end())
				tmp->_cardindex.insert(cardcache[j]);
			else {
				i--;
				cout << "You cannot bring duplicate cards, please choose again." << endl;
				break;
			}
		}
		charlist.push_back(*tmp);
	}

	cout << endl;
	cout << "Enter the map name: ";
	ss.str("");
	ss.clear();
	getline(cin, mapname);
	map1->SetMap(mapname);
}

