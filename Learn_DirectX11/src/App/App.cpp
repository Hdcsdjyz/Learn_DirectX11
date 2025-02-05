/**
 * @file App.cpp
 * @author Lhxl
 * @date 2025-2-5
 * @version build7
 */

#include <sstream>

#include "App.h"

App::App() : _wnd(1280, 720, L"SandTable") {}

int App::Go() {
	while (true) {
		if (const auto ecode = Window::ProcessMessage()) {
			return *ecode;
		}
		_DoFrame();
	}
}

void App::_DoFrame() {
	const float t = _timer.Peek();
	std::wostringstream woss;
	woss << L"耗时：" << std::setprecision(1) << std::fixed << t << "s";
	_wnd.SetTitle(woss.str());
}