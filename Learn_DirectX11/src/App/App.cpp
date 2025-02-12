/**
 * @file App.cpp
 * @author Lhxl
 * @date 2025-2-13
 * @version build16
 */

#include <sstream>

#include "App.h"

App::App() : _wnd(1280, 720, L"SandTable") {
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);
	for (auto i = 0; i < 50; i++) {
		_boxes.push_back(std::make_unique<Box>(_wnd.Gfx(), rng, adist, ddist, odist, rdist));
		_wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 100.0f));
	}
}

int App::Go() {
	while (true) {
		if (const auto ecode = Window::ProcessMessage()) {
			return *ecode;
		}
		_DoFrame();
	}
}

void App::_DoFrame() {
	auto dt = _timer.Peek();
	_wnd.Gfx().ClearBuffer(0.2f, 0.8f, 1.0f);
	for (auto& b : _boxes) {
		b->Update(0.001f * dt);
		b->Draw(_wnd.Gfx());
	}
	_wnd.Gfx().EndFrame();
}