/**
 * @file Window.cpp
 * @author Lhxl
 * @date 2025-2-5
 * @version build7
 */

#include <sstream>

#include "Window.h"
#include "../function.h"
#include "../resource/resource.h"

Window::_WindowClass Window::_WindowClass::_wndClass;

#pragma region class Window::WindowClass
Window::_WindowClass::_WindowClass() noexcept : _hInst(GetModuleHandle(nullptr)) {
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = _HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = static_cast<HICON>(LoadImage(_hInst, MAKEINTRESOURCE(IDI_ICON_TITLE), IMAGE_ICON, 32, 32, 0));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = static_cast<HICON>(LoadImage(_hInst, MAKEINTRESOURCE(IDI_ICON_TITLE), IMAGE_ICON, 16, 16, 0));
	RegisterClassEx(&wc);
}

Window::_WindowClass::~_WindowClass() {
	UnregisterClass(_wndClassName, GetInstance());
}

LPCWSTR Window::_WindowClass::GetName() noexcept {
	return _wndClassName;
}

HINSTANCE Window::_WindowClass::GetInstance() noexcept {
	return _wndClass._hInst;
}
#pragma endregion

#pragma region class Window
Window::Window(int width, int height, LPCWSTR name) : _width(width), _height(height) {
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0) {
		throw HWND_LAST_EXCEPT();
	}
	_hWnd = CreateWindow(
		_WindowClass::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT,
		wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr,
		_WindowClass::GetInstance(),
		this
	);
	if (_hWnd == nullptr) {
		throw HWND_LAST_EXCEPT();
	}
	ShowWindow(_hWnd, SW_SHOWDEFAULT);
}

Window::~Window() {
	DestroyWindow(_hWnd);
}

LRESULT WINAPI Window::_HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
	if (msg == WM_NCCREATE) {
		const CREATESTRUCT* const pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::_HandleMsgThunk));
		return pWnd->_HandleMsg(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::_HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWnd->_HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::_HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
	LPWSTR debugstring = mm(msg, lParam, wParam);
	OutputDebugString(debugstring);
	delptr(debugstring);
	switch (msg) {
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (!(lParam & 0x40000000) || kbd.IsAutorepeatEnable()) {
			kbd._OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		kbd._OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		kbd._OnChar(static_cast<unsigned char>(wParam));
		break;
	case WM_MOUSEMOVE:
		const POINTS pt0 = MAKEPOINTS(lParam);
		if (pt0.x >= 0 && pt0.x < _width && pt0.y >= 0 && pt0.y < _height) {
			mouse._OnMouseMove(pt0.x, pt0.y);
			if (!mouse.IsInWindow()) {
				SetCapture(hWnd);
				mouse._OnMouseEnter();
			}
		} else {
			if (wParam & (MK_LBUTTON | MK_RBUTTON)) {
				mouse._OnMouseMove(pt0.x, pt0.y);
			} else {
				ReleaseCapture();
				mouse._OnMouseLeave();
			}
		}
		break;
	case WM_LBUTTONDOWN:
		const POINTS pt1 = MAKEPOINTS(lParam);
		mouse._OnLBPressed(pt1.x, pt1.y);
		break;
	case WM_RBUTTONDOWN:
		const POINTS pt2 = MAKEPOINTS(lParam);
		mouse._OnRBPressed(pt2.x, pt2.y);
		break;
	case WM_LBUTTONUP:
		const POINTS pt3 = MAKEPOINTS(lParam);
		mouse._OnLBReleased(pt3.x, pt3.y);
		break;
	case WM_RBUTTONUP:
		const POINTS pt4 = MAKEPOINTS(lParam);
		mouse._OnRBReleased(pt4.x, pt4.y);
		break;
	case WM_MOUSEWHEEL:
		const POINTS pt5 = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse._OnWheelDelta(pt5.x, pt5.y, delta);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Window::SetTitle(const std::wstring& title) {
	if (SetWindowText(_hWnd, title.c_str()) == 0) {
		throw HWND_LAST_EXCEPT();
	}
}

std::optional<int> Window::ProcessMessage() {
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			return msg.wParam;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return {};
}
#pragma endregion

#pragma region class Window::Exception
std::wstring Window::_Exception::TranslateErrorCode(HRESULT hr) noexcept {
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
#pragma endregion

#pragma region class Window::HrException
Window::HrException::HrException(int line, LPCWSTR file, HRESULT hr) noexcept : _Exception(line, file), _hr(hr) {}

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
	return _hr;
}

std::wstring Window::HrException::GetErrorDescription() const noexcept {
	return TranslateErrorCode(_hr);
}
#pragma endregion