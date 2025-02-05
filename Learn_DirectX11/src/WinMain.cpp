/**
 * @file WinMain.cpp
 * @author Lhxl
 * @date 2025-2-3
 * @version build4
 */

#include <sstream>

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
			while (!wnd.mouse.IsEmpty()) {
				const auto e = wnd.mouse.Read();
				if (e.GetType() == Mouse::Event::Type::MOVE) {
					std::wostringstream woss;
					woss << "Mouse pos: (" << wnd.mouse.GetPosX() << ',' << wnd.mouse.GetPosY() << ')' << std::endl;
					wnd.SetTitle(woss.str());
				}
			}
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