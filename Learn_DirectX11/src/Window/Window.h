/**
 * @file Window.h
 * @auther Lhxl
 * @date 2025-2-3
 * @version build4
 */

#ifndef WINDOW_H
#define WINDOW_H

#include "ST_Window.h"
#include "../ST_Exception.h"

class Window {
public:
	Window(int width, int height, LPCWSTR name);
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
	class Exception : public ST_Exception {
		using ST_Exception::ST_Exception;
	public:
		static std::wstring TranslateErrorCode(HRESULT hr) noexcept;
	};
public:
	class HrException : public Exception {
	public:
		HrException(int line, LPCWSTR file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::wstring GetErrorDescription() const noexcept;
	private:
		HRESULT hr;
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

/**
 * @brief 抛出“ST_Exception”异常
 */
#define HWND_EXCEPT(hr) Window::Exception(__LINE__, __FILEW__, hr)
 /**
  * @brief 抛出最后一个异常
  */
#define HWND_LAST_EXCEPT() Window::HrException(__LINE__, __FILEW__, GetLastError())

#endif