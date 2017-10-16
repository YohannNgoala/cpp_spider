#pragma once

#ifndef MOUSEHOOKER_HPP_
# define MOUSEHOOKER_HPP_

#include "ADataHooker.hpp"

class							MouseHooker : public ADataHooker
{
public:
	MouseHooker() = default;
	~MouseHooker() = default;
	

	void						startHook();
	void						stopHook();
	static LRESULT	CALLBACK	mouseHookProc(int nCode, WPARAM wParam, LPARAM lParam);
	static void					mouseMoved(WPARAM wParam, std::pair<int, int> pos, PMSLLHOOKSTRUCT msStruct, char buff[MAXLENGTH]);
};

#endif // !MOUSEHOOKER_HPP_