/**
 * @file Window.h
 * @author Lhxl
 * @date 2025-2-6
 * @version build8
 */

#ifndef WINDOW_H
#define WINDOW_H

#include <optional>
#include <memory>

#include "ST_Window.h"
#include "MessageMap.h"
#include "../ST_Exception.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "../Graphics/Graphics.h"

class Window {
public:
	Window(int width, int height, LPCWSTR name);
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	~Window();
private:
	class _WindowClass {
	private:
		_WindowClass() noexcept;
		_WindowClass(const _WindowClass&) = delete;
		_WindowClass& operator=(const _WindowClass&) = delete;
		~_WindowClass();
	public:
		static LPCWSTR GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		static constexpr LPCWSTR _wndClassName = L"SandTable";
		static _WindowClass _wndClass;
		HINSTANCE _hInst;
	};
private:
	class _Exception : public ST_Exception {
		using ST_Exception::ST_Exception;
	public:
		static std::wstring TranslateErrorCode(HRESULT hr) noexcept;
	};
public:
	class HrException : public _Exception {
	public:
		HrException(int line, LPCWSTR file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::wstring GetErrorDescription() const noexcept;
	private:
		HRESULT _hr;
	};
private:
	static LRESULT WINAPI _HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT WINAPI _HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT _HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
public:
	void SetTitle(const std::wstring& title);
	static std::optional<int> ProcessMessage();
	Graphics& Gfx();
public:
	MessageMap mm;
	Keyboard kbd;
	Mouse mouse;
private:
	int _width;
	int _height;
	HWND _hWnd;
	std::unique_ptr<Graphics> _pGfx;
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