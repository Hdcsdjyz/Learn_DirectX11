/***
 * @file WinMain.cpp
 * @auther Lhxl
 * @date 2025-2-2
 * @version build1
 ***/

#include <Windows.h>

#include "MessageMap.h"

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	const auto pClassName = L"SandTable";
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = pClassName;
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);

	HWND hWnd = CreateWindowEx(
		0,
		pClassName,
		L"SandTable",
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		0, 0,
		640, 480,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);
	ShowWindow(hWnd, SW_SHOW);
	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, nullptr, NULL, NULL)) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if (gResult == -1) {
		return -1;
	}
	return 0;
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	static MessageMap mm;
	OutputDebugString(mm(msg, lParam, wParam));
	switch (msg) {
	case WM_CLOSE:
		PostQuitMessage(1);
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}