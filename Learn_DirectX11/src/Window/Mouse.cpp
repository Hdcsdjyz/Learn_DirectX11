/**
 * @file Mouse.cpp
 * @author Lhxl
 * @date 2025-2-5
 * @version build7
 */

#include "Mouse.h"
#include "ST_Window.h"

#pragma region class Mouse::Event
Mouse::Event::Event() noexcept
	: _type(Type::INVALID), _lbPressed(false), _rbPressed(false), _x(0), _y(0) {
}

Mouse::Event::Event(Type type, const Mouse& parent) noexcept
	: _type(type), _lbPressed(parent._lbPressed), _rbPressed(parent._rbPressed), _x(parent._x), _y(parent._y) {
}

bool Mouse::Event::IsValid() const noexcept {
	return _type != Type::INVALID;
}

Mouse::Event::Type Mouse::Event::GetType() const noexcept {
	return _type;
}

std::pair<int, int> Mouse::Event::GetPos() const noexcept {
	return { _x, _y };
}

int Mouse::Event::GetPosX() const noexcept {
	return _x;
}

int Mouse::Event::GetPosY() const noexcept {
	return _y;
}

bool Mouse::Event::IsLBPressed() const noexcept {
	return _lbPressed;
}

bool Mouse::Event::IsRBPressed() const noexcept {
	return _rbPressed;
}
#pragma endregion

#pragma region class Mouse
std::pair<int, int> Mouse::GetPos() const noexcept {
	return {_x, _y};
}

int Mouse::GetPosX() const noexcept {
	return _x;
}

int Mouse::GetPosY() const noexcept {
	return _y;
}

bool Mouse::IsLBPressed() const noexcept {
	return _lbPressed;
}

bool Mouse::IsRBPressed() const noexcept {
	return _rbPressed;
}

bool Mouse::IsInWindow() const noexcept {
	return _inWindow;
}

Mouse::Event Mouse::Read() noexcept {
	if (_buffer.size()) {
		Mouse::Event e = _buffer.front();
		_buffer.pop();
		return e;
	} else {
		return Mouse::Event();
	}
}

bool Mouse::IsEmpty() const noexcept {
	return _buffer.empty();
}

void Mouse::Flush() noexcept {
	_buffer = std::queue<Event>();
}

void Mouse::_OnMouseMove(int new_x, int new_y) noexcept {
	_x = new_x;
	_y = new_y;
	_buffer.push(Mouse::Event(Mouse::Event::Type::MOVE, *this));
	_TrimBuffer();
}

void Mouse::_OnLBPressed(int x, int y) noexcept {
	_lbPressed = true;
	_buffer.push(Mouse::Event(Mouse::Event::Type::LB_PRESS, *this));
	_TrimBuffer();
}

void Mouse::_OnLBReleased(int x, int y) noexcept {
	_lbPressed = false;
	_buffer.push(Mouse::Event(Mouse::Event::Type::LB_RELEASE, *this));
	_TrimBuffer();
}

void Mouse::_OnRBPressed(int x, int y) noexcept {
	_rbPressed = true;
	_buffer.push(Mouse::Event(Mouse::Event::Type::RB_PRESS, *this));
	_TrimBuffer();
}

void Mouse::_OnRBReleased(int x, int y) noexcept {
	_rbPressed = false;
	_buffer.push(Mouse::Event(Mouse::Event::Type::RB_RELEASE, *this));
	_TrimBuffer();
}

void Mouse::_OnWheelUp(int x, int y) noexcept {
	_buffer.push(Mouse::Event(Mouse::Event::Type::WHEEL_UP, *this));
	_TrimBuffer();
}

void Mouse::_OnWheelDown(int x, int y) noexcept {
	_buffer.push(Mouse::Event(Mouse::Event::Type::WHEEL_DOWN, *this));
	_TrimBuffer();
}

void Mouse::_OnMouseLeave() noexcept {
	_inWindow = false;
	_buffer.push(Mouse::Event(Mouse::Event::Type::LEAVE, *this));
	_TrimBuffer();
}

void Mouse::_OnMouseEnter() noexcept {
	_inWindow = true;
	_buffer.push(Mouse::Event(Mouse::Event::Type::ENTER, *this));
	_TrimBuffer();
}

void Mouse::_TrimBuffer() noexcept {
	while (_buffer.size() > _bufferSize) {
		_buffer.pop();
	}
}

void Mouse::_OnWheelDelta(int x, int y, int delta) noexcept {
	_wheelDelta += delta;
	while (_wheelDelta >= WHEEL_DELTA) {
		_wheelDelta -= WHEEL_DELTA;
		_OnWheelUp(x, y);
	}
	while (_wheelDelta <= -WHEEL_DELTA) {
		_wheelDelta += WHEEL_DELTA;
		_OnWheelDown(x, y);
	}
}
#pragma endregion