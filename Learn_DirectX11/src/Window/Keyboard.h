/**
 * @file Keyboard.h
 * @author Lhxl
 * @date 2025-2-5
 * @version build7
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <queue>
#include <bitset>

class Keyboard
{
	friend class Window;
public:
	class Event {
	public:
		enum class Type {
			PRESS,
			RELEASE,
			INVALID
		};
		Event();
		Event(Type type, unsigned char code) noexcept;
		bool IsPress() const noexcept;
		bool IsRelease() const noexcept;
		bool IsValid() const noexcept;
		unsigned char GetCode() const noexcept;
	private:
		Type _type;
		unsigned char _code;
	};
public:
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;
public:
	/* key */
	bool KeyIsPressed(unsigned char keycode) const noexcept;
	Event ReadKey() noexcept;
	bool IsKeyEmpty() const noexcept;
	void FlushKey() noexcept;
	/* char */
	char ReadChar() noexcept;
	bool IsCharEmpty() const noexcept;
	void FlushChar() noexcept;
	void Flush() noexcept;
	/* autorepeat */
	void EnableAutorepeat() noexcept;
	void DisableAutorepeat() noexcept;
	bool IsAutorepeatEnable() const noexcept;
private:
	void _OnKeyPressed(unsigned char keycode) noexcept;
	void _OnKeyReleased(unsigned char keycode) noexcept;
	void _OnChar(char character) noexcept;
	void _ClearState() noexcept;
	template<typename T>
	static void _TrimBuffer(std::queue<T>& buffer) noexcept;
private:
	static constexpr unsigned int _nKeys = 256u;
	static constexpr unsigned int _bufferSize = 16u;
	bool _autorepeat = false;
	std::bitset<_nKeys> _keystates;
	std::queue<Event> _keybuffer;
	std::queue<char> _charbuffer;
};

#endif