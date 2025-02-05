/**
 * @file Mouse.cpp
 * @author Lhxl
 * @date 2025-2-4
 * @version build6
 */

#include "Mouse.h"
#include "ST_Window.h"

#pragma region class Mouse::Event
Mouse::Event::Event() noexcept
	: type(Type::INVALID), lbPressed(false), rbPressed(false), x(0), y(0) {
}

Mouse::Event::Event(Type type, const Mouse& parent) noexcept
	: type(type), lbPressed(parent.lbPressed), rbPressed(parent.rbPressed), x(parent.x), y(parent.y) {
}

bool Mouse::Event::IsValid() const noexcept {
	return type != Type::INVALID;
}

Mouse::Event::Type Mouse::Event::GetType() const noexcept {
	return type;
}

std::pair<int, int> Mouse::Event::GetPos() const noexcept {
	return { x, y };
}

int Mouse::Event::GetPosX() const noexcept {
	return x;
}

int Mouse::Event::GetPosY() const noexcept {
	return y;
}

bool Mouse::Event::IsLBPressed() const noexcept {
	return lbPressed;
}

bool Mouse::Event::IsRBPressed() const noexcept {
	return rbPressed;
}
#pragma endregion

#pragma region class Mouse
std::pair<int, int> Mouse::GetPos() const noexcept {
	return {x, y};
}

int Mouse::GetPosX() const noexcept {
	return x;
}

int Mouse::GetPosY() const noexcept {
	return y;
}

bool Mouse::IsLBPressed() const noexcept {
	return lbPressed;
}

bool Mouse::IsRBPressed() const noexcept {
	return rbPressed;
}

bool Mouse::IsInWindow() const noexcept {
	return inWindow;
}

Mouse::Event Mouse::Read() noexcept {
	if (buffer.size()) {
		Mouse::Event e = buffer.front();
		buffer.pop();
		return e;
	} else {
		return Mouse::Event();
	}
}

bool Mouse::IsEmpty() const noexcept {
	return buffer.empty();
}

void Mouse::Flush() noexcept {
	buffer = std::queue<Event>();
}

void Mouse::OnMouseMove(int new_x, int new_y) noexcept {
	x = new_x;
	y = new_y;
	buffer.push(Mouse::Event(Mouse::Event::Type::MOVE, *this));
	TrimBuffer();
}

void Mouse::OnLBPressed(int x, int y) noexcept {
	lbPressed = true;
	buffer.push(Mouse::Event(Mouse::Event::Type::LB_PRESS, *this));
	TrimBuffer();
}

void Mouse::OnLBReleased(int x, int y) noexcept {
	lbPressed = false;
	buffer.push(Mouse::Event(Mouse::Event::Type::LB_RELEASE, *this));
	TrimBuffer();
}

void Mouse::OnRBPressed(int x, int y) noexcept {
	rbPressed = true;
	buffer.push(Mouse::Event(Mouse::Event::Type::RB_PRESS, *this));
	TrimBuffer();
}

void Mouse::OnRBReleased(int x, int y) noexcept {
	rbPressed = false;
	buffer.push(Mouse::Event(Mouse::Event::Type::RB_RELEASE, *this));
	TrimBuffer();
}

void Mouse::OnWheelUp(int x, int y) noexcept {
	buffer.push(Mouse::Event(Mouse::Event::Type::WHEEL_UP, *this));
	TrimBuffer();
}

void Mouse::OnWheelDown(int x, int y) noexcept {
	buffer.push(Mouse::Event(Mouse::Event::Type::WHEEL_DOWN, *this));
	TrimBuffer();
}

void Mouse::OnMouseLeave() noexcept {
	inWindow = false;
	buffer.push(Mouse::Event(Mouse::Event::Type::LEAVE, *this));
	TrimBuffer();
}

void Mouse::OnMouseEnter() noexcept {
	inWindow = true;
	buffer.push(Mouse::Event(Mouse::Event::Type::ENTER, *this));
	TrimBuffer();
}

void Mouse::TrimBuffer() noexcept {
	while (buffer.size() > bufferSize) {
		buffer.pop();
	}
}

void Mouse::OnWheelDelta(int x, int y, int delta) noexcept {
	wheelDelta += delta;
	while (wheelDelta >= WHEEL_DELTA) {
		wheelDelta -= WHEEL_DELTA;
		OnWheelUp(x, y);
	}
	while (wheelDelta <= -WHEEL_DELTA) {
		wheelDelta += WHEEL_DELTA;
		OnWheelDown(x, y);
	}
}
#pragma endregion