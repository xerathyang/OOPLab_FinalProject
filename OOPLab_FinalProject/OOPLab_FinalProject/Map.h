#pragma once

#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

class MapData {
public:
	MapData();
	void SetMap(string);
private:
	vector<vector<int>> _map;
	vector<int> _startx, _starty, _monsterx, _monstery;

	int _mapx, _mapy, _monstercount;
};

class Point2d {
public:
	Point2d();
	Point2d(int, int);

	int x() { return _x; }
	int y() { return _y; }

	Point2d operator+(const Point2d&);
	Point2d operator-(const Point2d&);
	//Point2d& operator++();
	//Point2d operator++(int);
	//Point2d& operator--();
	//Point2d operator--(int);
	bool operator==(const Point2d&);

private:
	int _x;
	int _y;
};