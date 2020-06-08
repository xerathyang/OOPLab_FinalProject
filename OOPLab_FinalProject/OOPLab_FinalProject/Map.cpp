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
	vector<char> tmp;
	fs.open(MapPath);
	fs >> _mapy >> _mapx;
	for (int i = 0; i < _mapy; i++) {
		fs >> input;
		tmp.clear();
		for (int j = 0; j < _mapx; j++) {
			tmp.push_back(input[j]);
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
		fs >> input >> _x >> _y;
		_monstername.push_back(input);
		Point2d monstercur(_x, _y);
		_monsterstart.push_back(monstercur);
		fs >> cache;
		_monsterc2.push_back(cache);
		fs >> cache;
		_monsterc3.push_back(cache);
		fs >> cache;
		_monsterc4.push_back(cache);
	}
	fs.close();
}

void MapData::printMap() {
	system("CLS");
	for (int i = 0; i < _mapy; i++) {
		for (int j = 0; j < _mapx; j++) {
			cout << _map[i][j];
		}
		cout << endl;
	}
}

void MapData::SetSymbol(Point2d& tar, char text) {
	_map[tar.y()][tar.x()] = text;
}

//generate the visible area
vector<vector<char>> MapData::generateVisiblefilter() {
	vector<vector<char>> tmp = _map;
	generatedriver(tmp, _start[0].x(), _start[0].y());
	return tmp;
}

//find area by recursive
void MapData::generatedriver(vector<vector<char>> &tar,int x,int y) {
	if (tar[y][x] == '1' || tar[y][x] == '2')
		tar[y][x] = '+';
	else
		return;

	//system("CLS");
	//for (int i = 0; i < tar.size(); i++) {
	//	for (int j = 0; j < tar[0].size(); j++) {
	//		cout << tar[i][j];
	//	}
	//	cout << endl;
	//}

	if (x > 0)
		generatedriver(tar, x - 1, y);
	if (y > 0)
		generatedriver(tar, x, y - 1);
	if ((unsigned)x < tar[0].size())
		generatedriver(tar, x + 1, y);
	if ((unsigned)y < tar.size())
		generatedriver(tar, x, y + 1);
}

bool  MapData::FindBarrier(Point2d p1, Point2d p2)
{
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
			_map[p1.x()][a] = 8;
		}
	}
	else if ((p1.y() == p2.y()))
	{
		for (int a = p1.x(); a <= p2.x(); a++)
		{
			_map[a][p1.y()] = 8;
		}
	}
	else
	{
		float  m = (p1.y() - p2.y()) / (p1.x() - p2.x());
		//y1<y2
		if (p1.y() < p2.y())
		{
			for (int b = p1.y(); b <= p2.y(); b++)
			{
				for (int a = p1.x(); a <= p2.x(); a++)
				{
					for (float t = 0; t <= 1; t += 0.001)
					{
						float xx = p1.x() + (p2.x() - p1.x())*t;
						float yy = p1.y() + (p2.y() - p1.y())*t;
						if ((float)a - 0.5 <= xx && xx <= (float)a + 0.5 && (float)b - 0.5 <= yy && yy <= (float)b + 0.5)
						{
							if (_map[a][b] == 0 || _map[a][b] == 3) {
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
					for (float t = 0; t <= 1; t += 0.001)
					{
						float xx = p1.x() + (p2.x() - p1.x())*t;
						float yy = p1.y() + (p2.y() - p1.y())*t;
						if ((float)a - 0.5 <= xx && xx <= (float)a + 0.5 && (float)b - 0.5 <= yy && yy <= (float)b + 0.5)
						{
							if (_map[a][b] == 0 || _map[a][b] == 3) {
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

///////////////

Point2d::Point2d() {
	_x = 0;
	_y = 0;
}

Point2d::Point2d(int inputx, int inputy) {
	_x = inputx;
	_y = inputy;
}

Point2d& Point2d::operator+(const Point2d& tar) {
	Point2d* tmp = new Point2d(_x + tar._x, _y + tar._y);
	return *tmp;
}

Point2d& Point2d::operator-(const Point2d& tar) {
	Point2d* tmp = new Point2d(_x - tar._x, _y - tar._y);
	return *tmp;
}

bool Point2d::operator==(const Point2d& tar) {
	if (_x == tar._x && _y == tar._y) {
		return true;
	}
	else {
		return false;
	}
}

bool Point2d::operator!=(const Point2d& tar) {
	if (_x == tar._x && _y == tar._y) {
		return false;
	}
	else {
		return true;
	}
}