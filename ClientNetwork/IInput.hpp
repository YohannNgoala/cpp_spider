#pragma once

#ifndef IINPUT_HPP_
# define IINPUT_HPP_

#include <utility>

class								IInput
{
public:
	virtual int						getKeyInput() = 0;
	virtual std::pair<int, int>		getMouseInput() = 0;
	virtual int						getTime() = 0;
	virtual char					*getProcessus() = 0;
};

#endif // !IINPUT_HPP_