/**
 * @file WinMain.cpp
 * @author Lhxl
 * @date 2025-2-5
 * @version build7
 */

#include <sstream>

#include "App/App.h"
//
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
	try {
		return App{}.Go();
	} catch (const ST_Exception& e) {
		MessageBox(nullptr, str2lpwstr(e.what()), str2lpwstr(e.GetType()), MB_OK | MB_ICONEXCLAMATION);
	} catch (const std::exception& e) {
		MessageBox(nullptr, str2lpwstr(e.what()), L"Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	} catch (...) {
		MessageBox(nullptr, L"No details available", L"Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}