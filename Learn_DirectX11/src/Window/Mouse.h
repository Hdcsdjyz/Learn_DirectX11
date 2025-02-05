/**
 * @file Mouse.h
 * @author Lhxl
 * @date 2025-2-5
 * @version build7
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
		Type _type;
		bool _lbPressed;
		bool _rbPressed;
		int _x;
		int _y;
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
	void _OnMouseMove(int x, int y) noexcept;
	void _OnLBPressed(int x, int y) noexcept;
	void _OnLBReleased(int x, int y) noexcept;
	void _OnRBPressed(int x, int y) noexcept;
	void _OnRBReleased(int x, int y) noexcept;
	void _OnWheelUp(int x, int y) noexcept;
	void _OnWheelDown(int x, int y) noexcept;
	void _OnMouseLeave() noexcept;
	void _OnMouseEnter() noexcept;
	void _TrimBuffer() noexcept;
	void _OnWheelDelta(int x, int y, int delta) noexcept;
private:
	static constexpr unsigned int _bufferSize = 16u;
	int _x = 0;
	int _y = 0;
	bool _lbPressed = false;
	bool _rbPressed = false;
	bool _inWindow = false;
	int _wheelDelta = 0;
	std::queue<Event> _buffer;
};

#endif