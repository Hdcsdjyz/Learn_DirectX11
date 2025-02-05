/**
 * @file Keyboard.cpp
 * @author Lhxl
 * @date 2025-2-5
 * @version build7
 */

#include"Keyboard.h"

#pragma region class Keyboard::Event
Keyboard::Event::Event()
	: _type(Type::INVALID), _code(0u) {
}

Keyboard::Event::Event(Type type, unsigned char code) noexcept
	: _type(type), _code(code) {
}

bool Keyboard::Event::IsPress() const noexcept {
	return _type == Type::PRESS;
}

bool Keyboard::Event::IsRelease() const noexcept {
	return _type == Type::RELEASE;
}

bool Keyboard::Event::IsValid() const noexcept {
	return _type != Type::INVALID;
}

unsigned char Keyboard::Event::GetCode() const noexcept {
	return _code;
}
#pragma endregion

#pragma region class Keyboard
bool Keyboard::KeyIsPressed(unsigned char keycode) const noexcept {
	return _keystates[keycode];
}

Keyboard::Event Keyboard::ReadKey() noexcept {
	if (_keybuffer.size() > 0u) {
		Keyboard::Event e = _keybuffer.front();
		_keybuffer.pop();
		return e;
	} else {
		return Keyboard::Event();
	}
}

bool Keyboard::IsKeyEmpty() const noexcept {
	return _keybuffer.empty();
}

void Keyboard::FlushKey() noexcept {
	_charbuffer = std::queue<char>();
}

char Keyboard::ReadChar() noexcept {
	if (_charbuffer.size() > 0u) {
		unsigned char charcode = _charbuffer.front();
		_charbuffer.pop();
		return charcode;
	} else {
		return 0;
	}
}

bool Keyboard::IsCharEmpty() const noexcept {
	return _charbuffer.empty();
}

void Keyboard::FlushChar() noexcept {
	_charbuffer = std::queue<char>();
}

void Keyboard::Flush() noexcept {
	FlushKey();
	FlushChar();
}

void Keyboard::EnableAutorepeat() noexcept {
	_autorepeat = true;
}

void Keyboard::DisableAutorepeat() noexcept {
	_autorepeat = false;
}

bool Keyboard::IsAutorepeatEnable() const noexcept {
	return _autorepeat;
}

void Keyboard::_OnKeyPressed(unsigned char keycode) noexcept {
	_keystates[keycode] = true;
	_keybuffer.push(Keyboard::Event(Keyboard::Event::Type::PRESS, keycode));
	_TrimBuffer(_keybuffer);
}

void Keyboard::_OnKeyReleased(unsigned char keycode) noexcept {
	_keystates[keycode] = false;
	_keybuffer.push(Keyboard::Event(Keyboard::Event::Type::RELEASE, keycode));
	_TrimBuffer(_keybuffer);
}

void Keyboard::_OnChar(char character) noexcept {
	_charbuffer.push(character);
	_TrimBuffer(_charbuffer);
}

void Keyboard::_ClearState() noexcept {
	_keystates.reset();
}

template<typename T>
static void Keyboard::_TrimBuffer(std::queue<T>& buffer) noexcept {
	while (buffer.size() > _bufferSize) {
		buffer.pop();
	}
}
#pragma endregion