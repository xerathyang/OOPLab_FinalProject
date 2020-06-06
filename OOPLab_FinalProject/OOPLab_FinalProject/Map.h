#pragma once

#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

class Point2d {
public:
	Point2d();
	Point2d(int, int);

	int x() { return _x; };
	int y() { return _y; };

	Point2d& operator+(const Point2d&);
	Point2d& operator-(const Point2d&);
	//Point2d& operator++();
	//Point2d operator++(int);
	//Point2d& operator--();
	//Point2d operator--(int);
	bool operator==(const Point2d&);
	bool operator!=(const Point2d&);

private:
	int _x;
	int _y;
};

class MapData {
public:
	MapData();
	void SetMap(string);
	void printMap();
	int x() { return _mapx; };
	int y() { return _mapy; };
	void SetSymbol(Point2d&, char);
	vector<vector<char>> getMap() { return _map; };
	vector<vector<char>> generateVisiblefilter();
	void generatedriver(vector<vector<char>>&, int, int);

	friend class Gloomhaven;
private:
	vector<vector<char>> _map;
	vector<Point2d> _start, _monster;
	vector<int> _monsterc2, _monsterc3, _monsterc4;
	vector<string> _monstername;

	int _mapx, _mapy, _monstercount;
};

