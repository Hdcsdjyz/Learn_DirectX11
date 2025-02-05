/**
 * @file Keyboard.h
 * @author Lhxl
 * @date 2025-2-4
 * @version build6
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
		Type type;
		unsigned char code;
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
	void OnKeyPressed(unsigned char keycode) noexcept;
	void OnKeyReleased(unsigned char keycode) noexcept;
	void OnChar(char character) noexcept;
	void ClearState() noexcept;
	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer) noexcept;
private:
	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int bufferSize = 16u;
	bool autorepeat = false;
	std::bitset<nKeys> keystates;
	std::queue<Event> keybuffer;
	std::queue<char> charbuffer;
};

#endif