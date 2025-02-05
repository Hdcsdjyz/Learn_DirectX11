/**
 * @file ST_Timer.cpp
 * @author Lhxl
 * @date 2025-2-5
 * @version build7
 */

#include "ST_Timer.h"
 
ST_Timer::ST_Timer() {
	_last = std::chrono::steady_clock::now();
}

float ST_Timer::Mark() {
	const auto old = _last;
	_last = std::chrono::steady_clock::now();
	const std::chrono::duration<float> frameTime = _last - old;
	return frameTime.count();
}

float ST_Timer::Peek() const {
	return std::chrono::duration<float>(std::chrono::steady_clock::now() - _last).count();
}