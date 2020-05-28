#include "Character.h"

using namespace std;

CharacterSkill::CharacterSkill() {
	_index = 0;
	_dex = 0;
	_action.clear();
}

CharacterSkill::CharacterSkill(ifstream& fs) {
	string input, cache;
	vector<Action> tmp;
	int var1, var2;
	stringstream ss;
	ss.str("");
	ss.clear();
	fs.ignore();
	getline(fs, input);
	ss << input;
	//get index and dexterity
	ss >> _index >> _dex;
	
	//get up and down part
	for (int j = 0; j < 2; j++) {
		ss >> cache;
		//infinite loop
		while (cache != "-") {
			if (cache == "move") {
				//var1: move distance
				ss >> var1;
				tmp.push_back(Action(0, var1, 0));
			}
			else if (cache == "attack") {
				//var1: attack damage
				ss >> var1;
				ss >> cache;
				if (cache == "range") {
					//var2: attack range
					ss >> var2;
					tmp.push_back(Action(1, var1, var2));
				}
				else {
					tmp.push_back(Action(1, var1, 0));
					continue;
				}
			}
			else if (cache == "heal") {
				//var1: heal value
				ss >> var1;
				tmp.push_back(Action(2, var1, 0));
			}
			else if (cache == "shield") {
				//var1: shield value
				ss >> var1;
				tmp.push_back(Action(3, var1, 0));
			}
			ss >> cache;
		}
		_action.push_back(tmp);
	}

}

Character::Character() {
	_name = "";
	_life = 0;
	_startcard = 0;
	_availablecard = 0;
}

Character::Character(ifstream& fs) {
	string comp;
	CharacterSkill* cache;
	fs >> _name >> _life >> _startcard >> _availablecard;
	for (int i = 0; i < _availablecard; i++) {
		cache = new CharacterSkill(fs);
		_charactercard.push_back(*cache);
	}
}

CharacterData::CharacterData() {
	_charTypecount = 0;
}

CharacterData::CharacterData(string CharacterPath) {
	ifstream fs;
	Character* cache;
	fs.open(CharacterPath);
	fs >> _charTypecount;
	for (int i = 0; i < _charTypecount; i++) {
		cache = new Character(fs);
		_charlist.push_back(*cache);
	}
}