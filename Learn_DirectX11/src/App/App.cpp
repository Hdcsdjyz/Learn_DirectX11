/**
 * @file App.cpp
 * @author Lhxl
 * @date 2025-2-12
 * @version build14
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
	const float color = _timer.Peek() / 3.0f + 1.5f;
	_wnd.Gfx().ClearBuffer(sin(color), cos(color), 0.8f);
	_wnd.Gfx().DrawTestTriangle(-_timer.Peek(), 0, 0);
	_wnd.Gfx().DrawTestTriangle(_timer.Peek(), _wnd.mouse.GetPosX() / 640.0f - 1, -_wnd.mouse.GetPosY() / 360.0f * 2 + 2);
	_wnd.Gfx().EndFrame();
}