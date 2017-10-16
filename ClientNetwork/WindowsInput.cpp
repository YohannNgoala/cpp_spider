#include "stdafx.h"
#include "WindowsInput.hpp"


WindowsInput::WindowsInput(int key, std::pair<int, int> pos, int time, char proc[256]) :
	_key(key), _pos(pos), _time(time)
{
	memset(_proc, '\0', sizeof(char) * 256);
	strncpy_s(_proc, proc, 256);
}