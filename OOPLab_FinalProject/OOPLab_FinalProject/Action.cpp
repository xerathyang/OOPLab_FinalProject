#include "Action.h"

using namespace std;

Action::Action() {
	_type = 0;
	_param1 = 0;
	_param2 = 0;
	_moveway = "";
	_ranged = false;
}

Action::Action(int type, string move) {
	_type = type;
	_param1 = 0;
	_param2 = 0;
	_moveway = move;
	_ranged = false;
}

Action::Action(int type, int param1, int param2, bool ranged) {
	_type = type;
	_param1 = param1;
	_param2 = param2;
	_moveway = "";
	_ranged = ranged;
}

void Action::printAction() {
	switch (_type)
	{
	case 0:
		if (_moveway != "") {
			cout << " move " << _moveway;
		}
		break;
	case 1:
		cout << " attack " << _param1;
		if (_ranged) {
			cout << " range " << _param2;
		}
		break;
	case 2:
		cout << " heal " << _param1;
		break;
	case 3:
		cout << " shield " << _param1;
		break;
	default:
		break;
	}
}