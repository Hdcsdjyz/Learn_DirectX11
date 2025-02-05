/**
 * @file ST_Timer.h
 * @author Lhxl
 * @date 2025-2-5
 * @version build7
 */

#ifndef ST_TIMER_H
#define ST_TIMER_H

#include <chrono>

class ST_Timer {
public:
	ST_Timer();
public:
	float Mark();
	float Peek() const;
private:
	std::chrono::steady_clock::time_point _last;
};

#endif