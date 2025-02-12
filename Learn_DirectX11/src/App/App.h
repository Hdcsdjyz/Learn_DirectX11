/**
 * @file App.h
 * @author Lhxl
 * @date 2025-2-13
 * @version build16
 */

#ifndef APP_H
#define APP_H

#include "../Window/Window.h"
#include "../Timer/ST_Timer.h"
#include "../Drawable/Box.h"

class App {
public:
	App();
	~App() = default;
public:
	int Go();
private:
	void _DoFrame();
private:
	Window _wnd;
	ST_Timer _timer;
	std::vector<std::unique_ptr<Box>> _boxes;
};

#endif