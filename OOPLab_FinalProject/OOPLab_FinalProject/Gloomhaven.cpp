#include "Gloomhaven.h"

Gloomhaven::Gloomhaven() {
	characterFile = "";
	monsterFile = "";
	characterNum = 0;
	DEBUG_MODE = 0;
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

	srand((unsigned)time(NULL));

	//FOR DEBUG MODE

	//cout << "Use Debug Mode?";
	//getline(cin, cache);
	//if (cache == "1") {
	//	DEBUG_MODE = 1;
	//}

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
		tmp->_mapid = 'A' + i;
		charlist.push_back(*tmp);
	}

	cout << endl;
	cout << "Enter the map name: ";
	ss.str("");
	ss.clear();
	getline(cin, mapname);
	map1->SetMap(mapname);

	visiblearea = map1->generateVisiblefilter();

	vector<int>::iterator iter;
	if (characterNum == 2) {
		iter = map1->_monsterc2.begin();
	}
	else if (characterNum == 3) {
		iter = map1->_monsterc3.begin();
	}
	else if (characterNum == 4) {
		iter = map1->_monsterc4.begin();
	}

	//spawn monster by map info
	for (int i = 0; i < map1->_monstercount; i++) {
		Object* tmp;
		if (iter[i] == 0)
			continue;
		else {
			tmp = new Object();
			tmp->spawn(md1->find(map1->_monstername[i]), iter[i] - 1);
			tmp->_pos = map1->_monsterstart[i];
			monstertype.insert(tmp->_name);
		}
		if (visiblearea[tmp->_pos.y()][tmp->_pos.x()] == '+')
			tmp->_isactive = true;
		tmp->_mapid = 'a' + i;

		monsterlist.push_back(*tmp);
	}


	//print the map with start point
	Point2d startcache = map1->_start[0];
	printMap(1, startcache);

	//get user input to modify start point
	for (unsigned c = 0; c < charlist.size(); c++) {

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
				else if (cache[i] == 'a')
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

void Gloomhaven::preparephrase() {
	Object* tar;
	char charname;
	bool flag = false;
	int cache1, cache2;
	string input;
	stringstream ss;
	set<int>::iterator ster;

	printMap(0);
	//character prepare
	while (getline(cin, input)) {
		ss.str("");
		ss.clear();
		ss << input;
		ss >> charname;
		if (charname < 'A' || charname>'D' || charname - 'A' >= characterNum) {
			cout << "No character's ID is fit." << endl;
			continue;
		}
		tar = &charlist[charname - 'A'];
		if (tar->_hasmoved) {
			cout << "This character has moved." << endl;
			continue;
		}
		ss >> input;

		if (input == "check") {
			if (tar->_cardindex.size() < 2) {
				cout << "This character only can take long break." << endl;
				continue;
			}
			cout << "hand: ";
			ster = tar->_cardindex.begin();
			if (ster != tar->_cardindex.end()) {
				cout << *ster;
				ster++;
			}
			while (ster!=tar->_cardindex.end()) {
				cout << ", " << *ster;
				ster++;
			}
			cout << " ; discard: ";
			ster = tar->_discardindex.begin();
			if (ster != tar->_discardindex.end()) {
				cout << *ster;
				ster++;
			}
			while (ster != tar->_discardindex.end()) {
				cout << ", " << *ster;
				ster++;
			}
			cout << endl;
		}
		//long break
		else if (input=="-1") {
			tar->_hasmoved = true;
			tar->_dex = 99;
			tar->_card1 = -1;
			tar->_card2 = -1;
		}
		//choose 2 card
		else if (ss>>cache2) {
			ss.str("");
			ss.clear();
			ss << input;
			ss >> cache1;
			if (cardcheck(*tar, cache1, cache2)) {
				tar->_card1 = cache1;
				tar->_dex = cd1->find(tar->_name).getdex(cache1);
				tar->_card2 = cache2;
				tar->_hasmoved = true;
			}
			else {
				cout << "This character doesn't have these cards now." << endl;
				continue;
			}
			
		}
		else {
			cout << "Unknow Command." << endl;
			continue;
		}

		//if all character have moved
		for (unsigned i = 0; i < charlist.size(); i++) {
			if (charlist[i]._hasmoved)
				flag = true;
			else {
				flag = false;
				break;
			}
		}
		if (flag)
			break;
	}

	//monster prepare
	cache1 = 0;
	cache2 = -1;
	set<string>::iterator miter = monstertype.begin();
	vector<Object>::iterator iter;
	while (miter != monstertype.end()) {
		iter = monsterlist.begin();
		while (iter != monsterlist.end()) {
			if (iter->_name == *miter && iter->_isactive) {
				break;
			}
			iter++;
		}
		if (iter == monsterlist.end()) {
			miter++;
			continue;
		}

		if (DEBUG_MODE == 1) {
			//ascend choose card
		}
		else {
			flag = false;
			while (!flag) {
				cache1 = rand() % 6;
				auto searchmcard = iter->_cardindex.find(cache1);
				if (searchmcard != iter->_cardindex.end()) {
					iter->_card1 = cache1;
					iter->_dex = md1->find(iter->_name).getdex(cache1);
					iter->_cardindex.erase(cache1);
					flag = true;
				}
			}
		}
		iter++;
		while (iter != monsterlist.end()) {
			if (iter->_name == *miter) {
				iter->_card1 = cache1;
				iter->_cardindex.erase(cache1);
			}
			iter++;
		}
		miter++;
	}
}

void Gloomhaven::actionphrase() {
	//printMap(0);
	cout << endl;

	//get sort by dex, actionline store the Object's copy, need find back to original Object
	vector<Object> actionline;
	vector<Object>::iterator objectiter;
	Object* tmp;
	for (unsigned i = 0; i < charlist.size(); i++) {
		objectiter = actionline.begin();
		tmp = new Object();
		*tmp = charlist[i];
		while (objectiter != actionline.end()) {
			if (tmp->_dex < objectiter->_dex) {
				actionline.insert(objectiter, *tmp);
				break;
			}
			else {
				objectiter++;
			}	
		}
		if (objectiter == actionline.end()) {
			actionline.push_back(*tmp);
		}
	}
	for (unsigned i = 0; i < monsterlist.size(); i++) {
		objectiter = actionline.begin();
		tmp = new Object();
		*tmp = monsterlist[i];
		if (!tmp->_isactive)
			continue;
		while (objectiter != actionline.end()) {
			if (tmp->_dex < objectiter->_dex) {
				actionline.insert(objectiter, *tmp);
				break;
			}
			else {
				objectiter++;
			}
		}
		if (objectiter == actionline.end()) {
			actionline.push_back(*tmp);
		}

	}

	//list action
	vector<Action>::iterator actioniter;
	vector<Action> actiontmp;
	for (unsigned i = 0; i < actionline.size(); i++) {
		if (actionline[i]._ismonster) {
			cout << actionline[i]._name << " " << actionline[i]._dex;
			actiontmp = md1->find(actionline[i]._name).getskill(actionline[i]._card1);
			actioniter = actiontmp.begin();
			while (actioniter != actiontmp.end()) {
				actioniter->printAction();
				actioniter++;
			}
			cout << endl;
		}
		else {
			cout << actionline[i]._mapid << " " << actionline[i]._dex << " "
				<< actionline[i]._card1 << " " << actionline[i]._card2 << endl;
		}
	}


	cout << "!" << endl;
}

//for normal print
void Gloomhaven::printMap(int mode) {
	system("CLS");
	MapData cachemap = *map1;

	for (unsigned c = 0; c < charlist.size(); c++) {
		if (charlist[c]._isactive)
			cachemap.SetSymbol(charlist[c]._pos, charlist[c]._mapid);
	}

	for (unsigned c = 0; c < monsterlist.size(); c++) {
		if (monsterlist[c]._isactive)
			cachemap.SetSymbol(monsterlist[c]._pos, monsterlist[c]._mapid);
	}


	vector<vector<char>> tmp = cachemap.getMap();
	int x = cachemap.x();
	int y = cachemap.y();

	for (int i = 0; i < y; i++) {
		for (int j = 0; j < x; j++) {
			if (tmp[i][j] != '0' && visiblearea[i][j] == '+')
				cout << tmp[i][j];
			else if (tmp[i][j] == '3')
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
			cachemap.SetSymbol(charlist[c]._pos, charlist[c]._mapid);
	}

	for (unsigned c = 0; c < monsterlist.size(); c++) {
		if (monsterlist[c]._isactive)
			cachemap.SetSymbol(monsterlist[c]._pos, monsterlist[c]._mapid);
	}


	vector<vector<char>> tmp = cachemap.getMap();
	int x = cachemap.x();
	int y = cachemap.y();

	for (int i = 0; i < y; i++) {
		for (int j = 0; j < x; j++) {
			if (tmp[i][j] != '0' && visiblearea[i][j] == '+')
				cout << tmp[i][j];
			else if (tmp[i][j] == '3')
				cout << tmp[i][j];
			else
				cout << " ";
		}
		cout << endl;
	}

	cout << "Enter the combination of \"wasd\" to move *, enter \"e\" to confirm the position." << endl;
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

bool Gloomhaven::cardcheck(Object& tar, int card1, int card2) {
	auto search1 = tar._cardindex.find(card1);
	auto search2 = tar._cardindex.find(card2);
	if (search1 == tar._cardindex.end() || search2 == tar._cardindex.end())
		return false;
	else
		return true;
}
