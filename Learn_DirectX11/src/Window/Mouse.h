/**
 * @file Mouse.h
 * @author Lhxl
 * @date 2025-2-4
 * @version build6
 */

#ifndef MOUSE_H
#define MOUSE_H

#include <queue>

class Mouse {
	friend class Window;
public:
	class Event {
	public:
		enum class Type {
			LB_PRESS,
			LB_RELEASE,
			RB_PRESS,
			RB_RELEASE,
			WHEEL_UP,
			WHEEL_DOWN,
			MOVE,
			ENTER,
			LEAVE,
			INVALID
		};
	public:
		Event() noexcept;
		Event(Type type, const Mouse& parent) noexcept;
	public:
		bool IsValid() const noexcept;
		Type GetType() const noexcept;
		std::pair<int, int> GetPos() const noexcept;
		int GetPosX() const noexcept;
		int GetPosY() const noexcept;
		bool IsLBPressed() const noexcept;
		bool IsRBPressed() const noexcept;
	private:
		Type type;
		bool lbPressed;
		bool rbPressed;
		int x;
		int y;
	};
public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
public:
	std::pair<int, int> GetPos() const noexcept;
	int GetPosX() const noexcept;
	int GetPosY() const noexcept;
	bool IsLBPressed() const noexcept;
	bool IsRBPressed() const noexcept;
	bool IsInWindow() const noexcept;
	Mouse::Event Read() noexcept;
	bool IsEmpty() const noexcept;
	void Flush() noexcept;
private:
	void OnMouseMove(int x, int y) noexcept;
	void OnLBPressed(int x, int y) noexcept;
	void OnLBReleased(int x, int y) noexcept;
	void OnRBPressed(int x, int y) noexcept;
	void OnRBReleased(int x, int y) noexcept;
	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;
	void OnMouseLeave() noexcept;
	void OnMouseEnter() noexcept;
	void TrimBuffer() noexcept;
	void OnWheelDelta(int x, int y, int delta) noexcept;
private:
	static constexpr unsigned int bufferSize = 16u;
	int x;
	int y;
	bool lbPressed = false;
	bool rbPressed = false;
	bool inWindow = false;
	int wheelDelta = 0;
	std::queue<Event> buffer;
};

#endif