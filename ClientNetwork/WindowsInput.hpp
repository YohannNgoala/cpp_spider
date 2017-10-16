#pragma once

#ifndef WINDOWSINPUT_HPP_
# define WINDOWSINPUT_HPP_

#include <string>
#include "IInput.hpp"

class					WindowsInput :
	public IInput
{
public:
	WindowsInput(int key, std::pair<int, int> pos, int time, char proc[256]);
	~WindowsInput() = default;

	/* GETTERS */
	int					getKeyInput() { return(_key); }
	std::pair<int, int>	getMouseInput() { return (_pos); }
	int					getTime() { return (_time); }
	char				*getProcessus() { return(_proc); }

	/* SETTERS */
	void				setKeyInput(int key) { _key = key; }
	void				setMouseInput(std::pair<int, int> pos) { _pos = pos; }
	void				setTime(int time) { _time = time; }
	void				setProcessus(const std::string& proc)
	{
		for (size_t i = 0; i < proc.length(); i++)
		{ _proc[i] = proc[i]; } 
	}

private:
	int					_key;
	std::pair<int, int> _pos;
	int					_time;
	char				_proc[256];
};

#endif // !WINDOWSINPUT_HPP_