#include "Action.h"

using namespace std;

Action::Action() {
	_type = 0;
	_param1 = 0;
	_param2 = 0;
}

Action::Action(int type, int param1, int param2) {
	_type = type;
	_param1 = param1;
	_param2 = param2;
}