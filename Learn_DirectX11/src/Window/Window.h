/**
 * @file Window.h
 * @auther Lhxl
 * @date 2025-2-3
 * @version build2
 */

#ifndef WINDOW_H
#define WINDOW_H

#include "ST_Window.h"

class Window {
public:
	Window(int width, int height, LPCWSTR name) noexcept;
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	~Window();
private:
	class WindowClass {
	private:
		WindowClass() noexcept;
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		~WindowClass();
	public:
		static LPCWSTR GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		static constexpr LPCWSTR wndClassName = L"SandTable";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};
private:
	static LRESULT WINAPI HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT WINAPI HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
private:
	int width;
	int height;
	HWND hWnd;
};

#endif