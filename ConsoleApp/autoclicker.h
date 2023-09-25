#pragma once
#include <Windows.h> // Windows API (mouse_event, Sleep ms)

int autoclicker(bool enabled, int cps) {
	int CPS;
	CPS = (1000 / cps) / 2; // We delen door 2 omdat we 2 keer een mouse_event doen (MOUSEEVENTF_LEFTDOWN, MOUSEEVENTF_LEFTUP)
	while (enabled) {
		if (GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(0x4C)) // CTRL + L
		{
			enabled = false;
			std::cout << "Autoclicker has been disabled!" << std::endl;
			break;
		}
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		Sleep(CPS);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		Sleep(CPS);
	}
	return 1;
}