#include "Gloomhaven.h"

using namespace std;

Gloomhaven::Gloomhaven() {
	characterFile = "";
	monsterFile = "";
	characterNum = 0;
	DEBUG_MODE = 1;
	round = 1;
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
				flag = false;
				break;
			}
			auto search = tmp->_cardindex.find(cardcache[j]);
			if (search == tmp->_cardindex.end())
				tmp->_cardindex.insert(cardcache[j]);
			//duplicate card
			else {
				i--;
				cout << "You cannot bring duplicate cards, please choose again." << endl;
				flag = false;
				break;
			}
		}

		if (!flag)
			continue;


		
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
	cout << "round " << round << ":" << endl;
	round++;
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
		if (tar->_isdead) {
			cout << "This character is not avaliable now." << endl;
		}
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

	//bug
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

		if (DEBUG_MODE == 0) {
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
		tmp = &charlist[i];
		while (objectiter != actionline.end()) {
			if (tmp->_dex < objectiter->_dex) {
				actionline.insert(objectiter, *tmp);
				break;
			}
			else if (tmp->_dex==objectiter->_dex) {
				if (cd1->find(tmp->_name).getdex(tmp->_card2) < cd1->find(objectiter->_name).getdex(objectiter->_card2)) {
					actionline.insert(objectiter, *tmp);
					break;
				}
				else if (cd1->find(tmp->_name).getdex(tmp->_card2) == cd1->find(objectiter->_name).getdex(objectiter->_card2)) {
					if (tmp->_mapid < objectiter->_mapid) {
						actionline.insert(objectiter, *tmp);
						break;
					}
				}
			}
			objectiter++;
			
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
			cout << actionline[i]._name << " ";
			if (actionline[i]._dex < 10)
				cout << "0";
			cout << actionline[i]._dex;;
			actiontmp = md1->find(actionline[i]._name).getskill(actionline[i]._card1);
			actioniter = actiontmp.begin();
			while (actioniter != actiontmp.end()) {
				actioniter->printAction();
				actioniter++;
			}
			cout << endl;
		}
		else {
			cout << actionline[i]._mapid << " ";
			if (actionline[i]._dex < 10)
				cout << "0";
			cout << actionline[i]._dex << " ";
			cout << actionline[i]._card1 << " " << actionline[i]._card2 << endl;
		}
	}


	//cout << "!" << endl;
	string moveway;
	Point2d aftermove;
	int count;
	string cache;
	stringstream ss;


	//action part
	for (unsigned actioncount = 0; actioncount < actionline.size(); actioncount++) {
		//monster
		if (actionline[actioncount]._ismonster) {
			actiontmp = md1->find(actionline[actioncount]._name).getskill(actionline[actioncount]._card1);
			actioniter = actiontmp.begin();
			while (actioniter != actiontmp.end()) {
				switch (actioniter->gettype()) {
				case 0:
					moveway = actioniter->getmoveway();
					aftermove = actionline[actioncount]._pos;
					for (int i = 0; i < moveway.length(); i++) {
						switch (moveway[i]) {
						case 'w':
							aftermove = aftermove - Point2d(0, 1);
							break;
						case 'a':
							aftermove = aftermove - Point2d(1, 0);
							break;
						case 's':
							aftermove = aftermove + Point2d(0, 1);
							break;
						case 'd':
							aftermove = aftermove + Point2d(1, 0);
							break;
						default:
							break;
						}
					}
					if (!isoccupied(aftermove)&&isvalidpos(aftermove)) {
						findbyId(actionline[actioncount]._mapid)._pos = aftermove;
					}
					printMap(0);
					break;
				case 1:
					MonsterFindAndAttack(findbyId(actionline[actioncount]._mapid), actioniter->getparam1(), actioniter->getparam2());
					break;
				case 2:
					findbyId(actionline[actioncount]._mapid).regen(actioniter->getparam1());
					cout << actionline[actioncount]._mapid << " heal " << actioniter->getparam1() << ", now hp is " << actionline[actioncount]._life << endl;
					break;
				case 3:
					findbyId(actionline[actioncount]._mapid)._shield = actioniter->getparam1();
					cout << actionline[actioncount]._mapid << " shield " << actioniter->getparam1() << " this turn" << endl;
					break;
				default:
					break;
				}
				actioniter++;
				if (DEBUG_MODE == 0)
					getchar();
				
			}
		}
		//character
		else {
			cout << actionline[actioncount]._mapid << "'s turn: card " << actionline[actioncount]._card1;
			if (actionline[actioncount]._card1 != -1) {
				cout << " " << actionline[actioncount]._card2 << endl;
				findbyId(actionline[actioncount]._mapid)._cardindex.erase(actionline[actioncount]._card1);
				findbyId(actionline[actioncount]._mapid)._cardindex.erase(actionline[actioncount]._card2);
				findbyId(actionline[actioncount]._mapid)._discardindex.insert(actionline[actioncount]._card1);
				findbyId(actionline[actioncount]._mapid)._discardindex.insert(actionline[actioncount]._card2);

			}
			else
				cout << endl;
			while (getline(cin, cache)) {
				ss.str("");
				ss.clear();
				if (cache == "check") {
					for (int i = 0; i < charlist.size(); i++) {
						if (charlist[i]._isactive && !charlist[i]._isdead) {
							cout << charlist[i]._mapid << "-hp: " << charlist[i]._life << ", shield: "
								<< charlist[i]._shield << endl;
						}
					}
					for (int i = 0; i < monsterlist.size(); i++) {
						if (monsterlist[i]._isactive && !monsterlist[i]._isdead) {
							cout << monsterlist[i]._mapid << "-hp: " << monsterlist[i]._life << ", shield: "
								<< monsterlist[i]._shield << endl;
						}
					}
					continue;
				}
				if (actionline[actioncount]._card1 == -1) {
					ss << cache;
					ss >> count;
					findbyId(actionline[actioncount]._mapid).shuffle(count);
					findbyId(actionline[actioncount]._mapid).regen(2);
					cout << actionline[actioncount]._mapid << "heal 2, now hp is" << actionline[actioncount]._life << endl;
					cout << "remove card: " << count << endl;
					break;
				}
				else {
					ss << cache[0];
					ss >> count;
					if (actionline[actioncount]._card1 != count && actionline[actioncount]._card2 != count) {
						cout << "You don't have this card now." << endl;
						continue;
					}
					if (count == actionline[actioncount]._card1) {
						if (cache[1] == 'u') {
							HandleAction(findbyId(actionline[actioncount]._mapid), cd1->find(actionline[actioncount]._name).getskill(actionline[actioncount]._card1)[0]);
							HandleAction(findbyId(actionline[actioncount]._mapid), cd1->find(actionline[actioncount]._name).getskill(actionline[actioncount]._card2)[1]);
						}
						else if (cache[1] == 'd') {
							HandleAction(findbyId(actionline[actioncount]._mapid), cd1->find(actionline[actioncount]._name).getskill(actionline[actioncount]._card1)[1]);
							HandleAction(findbyId(actionline[actioncount]._mapid), cd1->find(actionline[actioncount]._name).getskill(actionline[actioncount]._card2)[0]);
						}
					}
					else if (count == actionline[actioncount]._card2) {
						if (cache[1] == 'u') {
							HandleAction(findbyId(actionline[actioncount]._mapid), cd1->find(actionline[actioncount]._name).getskill(actionline[actioncount]._card2)[0]);
							HandleAction(findbyId(actionline[actioncount]._mapid), cd1->find(actionline[actioncount]._name).getskill(actionline[actioncount]._card1)[1]);
						}
						else if (cache[1] == 'd') {
							HandleAction(findbyId(actionline[actioncount]._mapid), cd1->find(actionline[actioncount]._name).getskill(actionline[actioncount]._card2)[1]);
							HandleAction(findbyId(actionline[actioncount]._mapid), cd1->find(actionline[actioncount]._name).getskill(actionline[actioncount]._card1)[0]);
						}
					}
					break;
				}

			}
			
			
		}
	}

}

void Gloomhaven::checkdoor() {
	for (int i = 0; i < monsterlist.size(); i++) {
		if (monsterlist[i]._isactive && !monsterlist[i]._isdead) {
			return;
		}
	}
	for (int i = 0; i < charlist.size(); i++) {
		Point2d tmp = charlist[i]._pos;
		if (charlist[i]._isactive && map1->getMap()[tmp.y()][tmp.x()] == '3' && visiblearea[tmp.y()][tmp.x()] == '+') {
			map1->SetSymbol(tmp, '1');
			visiblearea = map1->generateVisiblefilter();
			break;
		}
	}

	
}

void Gloomhaven::roundreset() {
	for (int i = 0; i < monsterlist.size(); i++) {
		if (monsterlist[i]._isactive && !monsterlist[i]._isdead) {
			monsterlist[i]._hasmoved = false;
			monsterlist[i]._shield = 0;
		}
	}
	for (int i = 0; i < charlist.size(); i++) {
		Point2d tmp = charlist[i]._pos;
		if (charlist[i]._isactive && !charlist[i]._isdead) {
			charlist[i]._hasmoved = false;
			charlist[i]._shield = 0;
		}
	}

}

void Gloomhaven::HandleAction(Object& tar, vector<Action>& action) {
	string cache;
	//int count;
	stringstream ss;
	Point2d aftermove;
	for (unsigned i = 0; i < action.size(); i++) {
		ss.str("");
		ss.clear();
		switch (action[i].gettype()) {
		case 0:
			getline(cin, cache);
			if (cache.length() > action[i].getparam1()) {
				cout << "error move!!!" << endl;
				i--;
				continue;
			}
			//stay at same position
			if (cache == "e") {
				continue;
			}

			aftermove = tar._pos;
			for (int step = 0; step < cache.size(); step++) {
				switch (cache[step]) {
				case 'e':
					break;
				case 'w':
					aftermove = aftermove - Point2d(0, 1);
					break;
				case 'a':
					aftermove = aftermove - Point2d(1, 0);
					break;
				case 's':
					aftermove = aftermove + Point2d(0, 1);
					break;
				case 'd':
					aftermove = aftermove + Point2d(1, 0);
					break;
				default:
					break;
				}
			}
			if (!isoccupied(aftermove) && isvalidpos(aftermove)) {
				findbyId(tar._mapid)._pos = aftermove;
				printMap(0);
			}
			else {
				cout << "error move!!!" << endl;
				i--;
				continue;
			}

			break;
		case 1:
			getline(cin, cache);
			//give up attack
			if (cache == "0") {
				continue;
			}
			if (cache.length() > 1 || cache[0] > 'z' || cache[0] < 'a') {
				cout << "error target!!!" << endl;
				i--;
				continue;
			}
			else if (findbyId(cache[0])._isdead || !findbyId(cache[0])._isactive || FindBarrier(tar._pos, findbyId(cache[0])._pos)) {
				cout << "error target!!!" << endl;
				i--;
				continue;
			}
			else {
				//melee
				if (tar._range <= 0 && Objectdistance(tar, findbyId(cache[0]), 1)) {
					findbyId(cache[0]).damage(tar._atk + action[i].getparam1());
					cout << tar._mapid << " attack " << findbyId(cache[0])._mapid << " " << action[i].getparam1()
						<< " damage, " << findbyId(cache[0])._mapid << " shield " << findbyId(cache[0])._shield
						<< ", " << findbyId(cache[0])._mapid << " remain " << findbyId(cache[0])._life << " hp" << endl;
				}
				//range
				else if(Objectdistance(tar, findbyId(cache[0]), tar._range+action[i].getparam2())){
					findbyId(cache[0]).damage(tar._atk + action[i].getparam1());
					cout << tar._mapid << " attack " << findbyId(cache[0])._mapid << " " << action[i].getparam1()
						<< " damage, " << findbyId(cache[0])._mapid << " shield " << findbyId(cache[0])._shield
						<< ", " << findbyId(cache[0])._mapid << " remain " << findbyId(cache[0])._life << " hp" << endl;
				}
				else {
					cout << "error target!!!" << endl;
					i--;
					continue;

				}
				if (findbyId(cache[0])._life <= 0) {
					findbyId(cache[0])._isdead = true;
					printMap(0);
					cout << findbyId(cache[0])._mapid << " is killed!!" << endl;
				}
			}
			break;
		case 2:
			findbyId(tar._mapid).regen(action[i].getparam1());
			cout << tar._mapid << " heal " << action[i].getparam1() << ", now hp is " << tar._life << endl;
			break;
		case 3:
			findbyId(tar._mapid)._shield = action[i].getparam1();
			cout << tar._mapid << " shield " << tar._shield << " this turn." << endl;
			break;
		}
	}
}

//for normal print
void Gloomhaven::printMap(int mode) {
	system("CLS");
	MapData cachemap = *map1;

	for (unsigned c = 0; c < charlist.size(); c++) {
		if (charlist[c]._isactive && !charlist[c]._isdead)
			cachemap.SetSymbol(charlist[c]._pos, charlist[c]._mapid);
	}

	for (unsigned c = 0; c < monsterlist.size(); c++) {
		if (monsterlist[c]._isactive && !monsterlist[c]._isdead)
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

bool Gloomhaven::isvalidpos(Point2d& tar) {
	if (tar.x() < 0 || tar.y() < 0 || tar.x() >= map1->_mapx || tar.y() >= map1->_mapy)
		return false;
	if (map1->_map[tar.y()][tar.x()] == '1' || map1->_map[tar.y()][tar.x()] == '3')
		return true;
	else
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

Object& Gloomhaven::findbyId(char id) {
	if (id >= 'a') {
		for (unsigned i = 0; i < monsterlist.size(); i++) {
			if (monsterlist[i]._mapid == id) {
				return monsterlist[i];
			}
		}
	}
	else {
		for (unsigned i = 0; i < charlist.size(); i++) {
			if (charlist[i]._mapid == id) {
				return charlist[i];
			}
		}
	}
	Object* tmp = new Object();
	return *tmp;
}


//Ken
bool CompareForMFA(const pair<Object&, int> &a, const pair<Object&, int> &b)
{
	if (a.second == b.second)
	{
		if (a.first.getdex() == b.first.getdex())
		{
			return a.first.getname() < b.first.getname();
		}
		else
		{
			return a.first.getdex() < b.first.getdex();
		}
	}
	return a.second < b.second;
}

void Gloomhaven::MonsterFindAndAttack(Object &mon,int atk,int range)
{

	vector<pair<Object&, int>> tempCharlist;
	for (unsigned MFA = 0; MFA < charlist.size(); MFA++)
	{
		int X = charlist[MFA]._pos.x() - mon._pos.x();
		int Y = charlist[MFA]._pos.y() - mon._pos.y();
		if (abs(X) + abs(Y) <= (mon._range + range) && !FindBarrier(mon._pos, charlist[MFA]._pos))
		{
			pair<Object&, int> CharWithRange(charlist[MFA], abs(X) + abs(Y));
			tempCharlist.push_back(CharWithRange);
		}
	}
	if (tempCharlist.size() == 0) { return; }


	sort(tempCharlist.begin(), tempCharlist.end(), CompareForMFA);
	vector<pair<Object&, int>>::iterator pairiter = tempCharlist.begin();

	cout << mon._mapid << " lock " << pairiter->first._mapid << " in distance " << pairiter->second << endl;
	if (DEBUG_MODE == 0)
		getchar();
	int dmg = mon._atk + atk - pairiter->first._shield;
	if (dmg < 0)
		dmg = 0;
	pairiter->first._life = pairiter->first._life - dmg;
	cout << mon._mapid << " attack " << pairiter->first._mapid << " " << dmg << " damage,"
		<< pairiter->first._mapid << " shield " << pairiter->first._shield << ", "
		<< pairiter->first._mapid << " remain " << pairiter->first._life << " hp" << endl;
	if (DEBUG_MODE == 0)
		getchar();
	//check target life is positive or not
	if (pairiter->first._life <= 0) {
		pairiter->first._isdead = true;
		printMap(0);
		cout << pairiter->first._mapid << " is killed!!" << endl;
	}
}

bool Gloomhaven::FindBarrier(Point2d p1, Point2d p2)
{
	
	vector<vector<char>> tempMap = map1->getMap();
	if (p1.x() > p2.x())
	{
		Point2d temp;
		temp = p1;
		p1 = p2;
		p2 = temp;
		
	}
	if ((p1.x() == p2.x()))
	{
		for (int a = p1.y(); a <= p2.y(); a++)
		{
			if (tempMap[a][p1.x()] == 0 || tempMap[a][p1.x()] == 3) {
				return true;
			}
			
		}
	}
	else if ((p1.y() == p2.y()))
	{
		for (int a = p1.x(); a <= p2.x(); a++)
		{
			if (tempMap[p1.y()][a] == 0 || tempMap[p1.y()][a] == 3) {
				return true;
			}
		}
	}
	else
	{
		double  m = (p1.y() - p2.y()) / (p1.x() - p2.x());
		//y1<y2
		if (p1.y() < p2.y())
		{
			for (int b = p1.y(); b <= p2.y(); b++)
			{
				for (int a = p1.x(); a <= p2.x(); a++)
				{
					for (double t = 0; t <= 1; t += 0.001)
					{
						double xx = p1.x() + (p2.x() - p1.x())*t;
						double yy = p1.y() + (p2.y() - p1.y())*t;
						if ((double)a - 0.5 <= xx && xx <= (double)a + 0.5 && (double)b - 0.5 <= yy && yy <= (double)b + 0.5)
						{
							if (tempMap[b][a] == 0 || tempMap[b][a] == 3) {
								return true;
							}
						}
					}
				}
				//cout << "s" << endl;
			}

		}
		//y1>=y2
		if (p1.y() >= p2.y())
		{
			for (int b = p1.y(); b >= p2.y(); b--)
			{
				for (int a = p1.x(); a <= p2.x(); a++)
				{
					for (double t = 0; t <= 1; t += 0.001)
					{
						double xx = p1.x() + (p2.x() - p1.x())*t;
						double yy = p1.y() + (p2.y() - p1.y())*t;
						if ((double)a - 0.5 <= xx && xx <= (double)a + 0.5 && (double)b - 0.5 <= yy && yy <= (double)b + 0.5)
						{
							if (tempMap[b][a] == 0 || tempMap[b][a] == 3) {
								return true;
							}
						}
					}
				}
				//cout << "s" << endl;
			}

		}
	}
	return false;
}

bool Gloomhaven::Objectdistance(Object& tar1, Object& tar2, int dis) {
	int X = tar1._pos.x() - tar2._pos.x();
	int Y = tar1._pos.y() - tar2._pos.y();
	if (abs(X) + abs(Y) <= dis)
		return true;
	else
		return false;

}