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
			//card no exist
			if (cardcache[j] >= tmp->_avaliablecard) {
				i--;
				cout << "This character doesn't have \"" << cardcache[j] << "\"." << endl;
				break;
			}
			auto search = tmp->_cardindex.find(cardcache[j]);
			if (search == tmp->_cardindex.end())
				tmp->_cardindex.insert(cardcache[j]);
			//duplicate card
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

	//print the map with start point
	Point2d startcache = map1->_start[0];
	printMap(1, startcache);

	//get user input to modify start point
	for (unsigned c = 0; c < charlist.size(); c++) {

		//in progress
		unsigned startpush = 1;
		startcache = map1->_start[0];
		for (unsigned t = 0; t < c; t++) {
			while (startcache == charlist[t]._pos) {
				startcache = map1->_start[startpush];
				startpush++;
			}
		}

		//print initial map
		printMap(1, startcache);

		while (getline(cin, cache)) {
			//point select confirm
			if (cache == "e") {
				charlist[c]._pos = startcache;
				charlist[c]._isactive = true;
				printMap(1, startcache);
				break;
			}

			Point2d tostart = startcache;

			//set temp position
			for (unsigned i = 0; i < cache.size(); i++) {
				if (cache[i] == 'w')
					tostart = tostart - Point2d(0, 1);
				else if(cache[i]=='a')
					tostart = tostart - Point2d(1, 0);
				else if (cache[i] == 's')
					tostart = tostart + Point2d(0, 1);
				else if (cache[i] == 'd')
					tostart = tostart + Point2d(1, 0);
			}
			//if position is fit, move start point
			for (unsigned i = 0; i < map1->_start.size(); i++) {
				if (isoccupied(tostart)) {
					break;
				}
				if (tostart == map1->_start[i] && startcache != map1->_start[i]) {
					startcache = map1->_start[i];
					break;
				}
			}

			//print the modified map
			printMap(1, startcache);
		}

	}
	
}

void Gloomhaven::printMap(int mode) {
	system("CLS");
	MapData cachemap(*map1);



	vector<vector<char>> tmp = cachemap.getMap();
	int x = cachemap.x();
	int y = cachemap.y();

	for (int i = 0; i < y; i++) {
		for (int j = 0; j < x; j++) {
			if (tmp[i][j] != '0')
				cout << tmp[i][j];
			else
				cout << " ";
		}
		cout << endl;
	}
}

//for select start point
void Gloomhaven::printMap(int mode, Point2d& para1) {
	system("CLS");
	MapData cachemap = *map1;
	if (mode == 1) {
		for (unsigned int i = 0; i < cachemap._start.size(); i++) {
			cachemap.SetSymbol(cachemap._start[i], '_');
		}
		cachemap.SetSymbol(para1, '*');
	}
	for (unsigned c = 0; c < charlist.size(); c++) {
		if (charlist[c]._isactive)
			cachemap.SetSymbol(charlist[c]._pos, 'a' + c);
	}

	vector<vector<char>> tmp = cachemap.getMap();
	int x = cachemap.x();
	int y = cachemap.y();

	for (int i = 0; i < y; i++) {
		for (int j = 0; j < x; j++) {
			if (tmp[i][j] != '0')
				cout << tmp[i][j];
			else
				cout << " ";
		}
		cout << endl;
	}
}

//for check if the position is occupied by other object 
bool Gloomhaven::isoccupied(Point2d& tar) {
	for (unsigned i = 0; i < charlist.size(); i++) {
		if (charlist[i]._pos == tar)
			return true;
	}
	for (unsigned i = 0; i < monsterlist.size(); i++) {
		if (charlist[i]._pos == tar)
			return true;
	}
	return false;
}
