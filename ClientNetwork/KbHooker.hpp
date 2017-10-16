#pragma once

#ifndef KBHOOKER_HPP_
# define KBHOOKER_HPP_

#include			"ADataHooker.hpp"

class				KbHooker : public ADataHooker
{
public:
	KbHooker();
	~KbHooker() = default;

	void						startHook();
	void						stopHook();
	static LRESULT CALLBACK		keyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam);
};

#endif // !KBHOOKER_HPP_