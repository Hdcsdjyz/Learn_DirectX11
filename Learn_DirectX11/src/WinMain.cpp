/**
 * @file WinMain.cpp
 * @auther Lhxl
 * @date 2025-2-3
 * @version build2
 */

#include "Window/Window.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	Window wnd(640, 480, L"SandTable");
	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, nullptr, NULL, NULL)) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if (gResult == -1) {
		return -1;
	}
	return msg.wParam;
}