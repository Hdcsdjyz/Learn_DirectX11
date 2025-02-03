/**
 * @file WinMain.cpp
 * @auther Lhxl
 * @date 2025-2-3
 * @version build4
 */

#include "Window/Window.h"
#include "ST_Exception.h"
#include "function.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	try {
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
	} catch (const ST_Exception& e) {
		MessageBox(nullptr, str2lpwstr(e.what()), str2lpwstr(e.GetType()), MB_OK | MB_ICONEXCLAMATION);
	} catch (const std::exception& e) {
		MessageBox(nullptr, str2lpwstr(e.what()), L"Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	} catch (...) {
		MessageBox(nullptr, L"No details available", L"Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}