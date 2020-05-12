#include "Map.h"

MapData::MapData() {
	_map.clear();
	_mapx = 0;
	_mapy = 0;
	_monstercount = 0;
}

void MapData::SetMap(string MapPath) {
	ifstream fs;
	int _x, _y, cache;
	string input;
	vector<int> tmp;
	fs.open(MapPath);
	fs >> _mapy >> _mapx;
	for (int i = 0; i < _mapy; i++) {
		fs >> input;
		tmp.clear();
		for (int j = 0; j < _mapx; j++) {
			tmp.push_back(input[j] - '0');
		}
		_map.push_back(tmp);
	}
	for (int i = 0; i < 4; i++) {
		fs >> _x >> _y;
		Point2d playercur(_x, _y);
		_start.push_back(playercur);
	}
	fs >> cache;
	_monstercount = cache;
	for (int i = 0; i < _monstercount; i++) {
		fs >> _x >> _y;
		Point2d monstercur(_x, _y);
		_monster.push_back(monstercur);
		fs >> cache;
		_monsterc2.push_back(cache);
		fs >> cache;
		_monsterc3.push_back(cache);
		fs >> cache;
		_monsterc4.push_back(cache);
	}
	fs.close();
}

Point2d::Point2d() {
	_x = 0;
	_y = 0;
}

Point2d::Point2d(int inputx, int inputy) {
	_x = inputx;
	_y = inputy;
}

Point2d Point2d::operator+(const Point2d& tar) {
	Point2d tmp(_x + tar._x, _y + tar._y);
	return tmp;
}

Point2d Point2d::operator-(const Point2d& tar) {
	Point2d tmp(_x - tar._x, _y - tar._y);
	return tmp;
}

bool Point2d::operator==(const Point2d& tar) {
	if (_x == tar._x && _y == tar._y) {
		return true;
	}
	else {
		return false;
	}
}