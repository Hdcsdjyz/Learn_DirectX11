/**
 * @file Window.cpp
 * @auther Lhxl
 * @date 2025-2-3
 * @version build4
 */

#include <sstream>

#include "Window.h"
#include "MessageMap.h"
#include "../function.h"

Window::WindowClass Window::WindowClass::wndClass;

Window::Window(int width, int height, LPCWSTR name) : width(width), height(height) {
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0) {
		throw HWND_LAST_EXCEPT();
	}
	hWnd = CreateWindow(
		WindowClass::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT,
		wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr,
		WindowClass::GetInstance(),
		this
	);
	if (hWnd == nullptr) {
		throw HWND_LAST_EXCEPT();
	}
	ShowWindow(hWnd, SW_SHOWDEFAULT);
}

Window::~Window() {
	DestroyWindow(hWnd);
}

Window::WindowClass::WindowClass() noexcept : hInst(GetModuleHandle(nullptr)) {
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass() {
	UnregisterClass(wndClassName, GetInstance());
}

LPCWSTR Window::WindowClass::GetName() noexcept {
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept {
	return wndClass.hInst;
}

LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
	if (msg == WM_NCCREATE) {
		const CREATESTRUCT* const pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
	MessageMap mm;
	LPWSTR debugstring = mm(msg, lParam, wParam);
	OutputDebugString(debugstring);
	delptr(debugstring);

	switch (msg) {
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

std::wstring Window::Exception::TranslateErrorCode(HRESULT hr) noexcept {
	LPWSTR pMsgBuf = nullptr;
	const DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		hr,
		MAKELANGID(LANG_CHINESE_SIMPLIFIED, SUBLANG_CHINESE_SIMPLIFIED),
		reinterpret_cast<LPWSTR>(&pMsgBuf),
		0,
		nullptr
	);
	if (nMsgLen == 0) {
		return L"Unidentified error code";
	}
	std::wstring errorString = pMsgBuf;
	LocalFree(pMsgBuf);
	return errorString;
}

Window::HrException::HrException(int line, LPCWSTR file, HRESULT hr) noexcept : Exception(line, file), hr(hr) {

}

const char* Window::HrException::what() const noexcept {
	std::wostringstream oss;
	oss << GetType() << std::endl
		<< "[ERROR Code]: 0x" << std::hex << std::uppercase << GetErrorCode() << std::dec << "(" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Description]: " << GetErrorDescription() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	LPCWSTR w_str = whatBuffer.c_str();
	return lpwstr2lpcstr(w_str);
}

const char* Window::HrException::GetType() const noexcept {
	return "ST_Window_Exception";
}

HRESULT Window::HrException::GetErrorCode() const noexcept {
	return hr;
}

std::wstring Window::HrException::GetErrorDescription() const noexcept {
	return TranslateErrorCode(hr);
}