/**
 * @file Keyboard.cpp
 * @author Lhxl
 * @date 2025-2-4
 * @version build6
 */

#include"Keyboard.h"

#pragma region class Keyboard::Event
Keyboard::Event::Event()
	: type(Type::INVALID), code(0u) {
}

Keyboard::Event::Event(Type type, unsigned char code) noexcept
	:type(type), code(code) {
}

bool Keyboard::Event::IsPress() const noexcept {
	return type == Type::PRESS;
}

bool Keyboard::Event::IsRelease() const noexcept {
	return type == Type::RELEASE;
}

bool Keyboard::Event::IsValid() const noexcept {
	return type != Type::INVALID;
}

unsigned char Keyboard::Event::GetCode() const noexcept {
	return code;
}
#pragma endregion

#pragma region class Keyboard
bool Keyboard::KeyIsPressed(unsigned char keycode) const noexcept {
	return keystates[keycode];
}

Keyboard::Event Keyboard::ReadKey() noexcept {
	if (keybuffer.size() > 0u) {
		Keyboard::Event e = keybuffer.front();
		keybuffer.pop();
		return e;
	} else {
		return Keyboard::Event();
	}
}

bool Keyboard::IsKeyEmpty() const noexcept {
	return keybuffer.empty();
}

void Keyboard::FlushKey() noexcept {
	charbuffer = std::queue<char>();
}

char Keyboard::ReadChar() noexcept {
	if (charbuffer.size() > 0u) {
		unsigned char charcode = charbuffer.front();
		charbuffer.pop();
		return charcode;
	} else {
		return 0;
	}
}

bool Keyboard::IsCharEmpty() const noexcept {
	return charbuffer.empty();
}

void Keyboard::FlushChar() noexcept {
	charbuffer = std::queue<char>();
}

void Keyboard::Flush() noexcept {
	FlushKey();
	FlushChar();
}

void Keyboard::EnableAutorepeat() noexcept {
	autorepeat = true;
}

void Keyboard::DisableAutorepeat() noexcept {
	autorepeat = false;
}

bool Keyboard::IsAutorepeatEnable() const noexcept {
	return autorepeat;
}

void Keyboard::OnKeyPressed(unsigned char keycode) noexcept {
	keystates[keycode] = true;
	keybuffer.push(Keyboard::Event(Keyboard::Event::Type::PRESS, keycode));
	TrimBuffer(keybuffer);
}

void Keyboard::OnKeyReleased(unsigned char keycode) noexcept {
	keystates[keycode] = false;
	keybuffer.push(Keyboard::Event(Keyboard::Event::Type::RELEASE, keycode));
	TrimBuffer(keybuffer);
}

void Keyboard::OnChar(char character) noexcept {
	charbuffer.push(character);
	TrimBuffer(charbuffer);
}

void Keyboard::ClearState() noexcept {
	keystates.reset();
}

template<typename T>
static void Keyboard::TrimBuffer(std::queue<T>& buffer) noexcept {
	while (buffer.size() > bufferSize) {
		buffer.pop();
	}
}
#pragma endregion