#include "MouseHooker.hpp"

void			MouseHooker::startHook(void)
{
	if (!(_hooker = SetWindowsHookEx(WH_MOUSE_LL, mouseHookProc, NULL, 0)))
		return;
}


void			MouseHooker::stopHook(void) {
	UnhookWindowsHookEx(_hooker);
}

void				MouseHooker::mouseMoved(WPARAM wParam, std::pair<int, int> pos, PMSLLHOOKSTRUCT msStruct, char buff[MAXLENGTH]) {
	static  ULONG	lastMooveTime = 0; // Last mouvement of the mouse (regulate logging data)
	int				currTime = MouseHooker::getCurrTimestamp();
	if (!MouseHooker::checkIsActivated(gControlData->_state.isTrackingActivated()))
		return;
	if (lastMooveTime + 1 < currTime) {
		auto newInput = std::make_shared<WindowsInput>(static_cast<int>(wParam), pos, currTime, buff);
		MouseHooker::pushMyData(newInput);
		lastMooveTime = currTime;
	}
}

LRESULT CALLBACK		MouseHooker::mouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (!MouseHooker::checkIsActivated(gControlData->_state.isClickActivated())
		&& !MouseHooker::checkIsActivated(gControlData->_state.isTrackingActivated()))
		return (CallNextHookEx(NULL, nCode, wParam, lParam));

	PMSLLHOOKSTRUCT msStruct = (PMSLLHOOKSTRUCT)(lParam); //Pointer on mouse Struct
	HWND winHandle = GetForegroundWindow(); // Handle of actual window		
	char buff[MAXLENGTH];
	std::pair<int, int> pos(msStruct->pt.x, msStruct->pt.y);

	memset(buff, 0, sizeof(buff));
	GetWindowText(winHandle, buff, MAXLENGTH); // Transform window handle to readable data //
	if (nCode == 0) {
		if (wParam == WM_MOUSEMOVE) {
			mouseMoved(wParam, pos, msStruct, buff);
			return CallNextHookEx(NULL, nCode, wParam, lParam);
		}
		if (wParam == WM_LBUTTONDOWN
			|| wParam == WM_RBUTTONDOWN
			|| wParam == WM_MOUSEWHEEL) {
			auto newInput = std::make_shared<WindowsInput>(static_cast<int>(wParam), pos, MouseHooker::getCurrTimestamp(), buff);
			MouseHooker::pushMyData(newInput);
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}