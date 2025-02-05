/**
 * @file App.h
 * @author Lhxl
 * @date 2025-2-5
 * @version build7
 */

#ifndef APP_H
#define APP_H

#include "../Window/Window.h"
#include "../Timer/ST_Timer.h"

class App {
public:
	App();
public:
	int Go();
private:
	void _DoFrame();
private:
	Window _wnd;
	ST_Timer _timer;
};

#endif