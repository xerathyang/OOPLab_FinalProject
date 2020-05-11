#include "Map.h"

MapData::MapData() {
	_map.clear();
	_mapx = 0;
	_mapy = 0;
}

void MapData::SetMap(string MapPath) {
	ifstream fs;
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