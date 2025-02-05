/**
 * @file Window.cpp
 * @author Lhxl
 * @date 2025-2-3
 * @version build4
 */

#include <sstream>

#include "Window.h"
#include "../function.h"
#include "../resource/resource.h"

Window::WindowClass Window::WindowClass::wndClass;

#pragma region class Window::WindowClass
Window::WindowClass::WindowClass() noexcept : hInst(GetModuleHandle(nullptr)) {
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON_TITLE), IMAGE_ICON, 32, 32, 0));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON_TITLE), IMAGE_ICON, 16, 16, 0));
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
#pragma endregion

#pragma region class Window
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
			kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		kbd.OnChar(static_cast<unsigned char>(wParam));
		break;
	case WM_MOUSEMOVE:
		const POINTS pt0 = MAKEPOINTS(lParam);
		if (pt0.x >= 0 && pt0.x < width && pt0.y >= 0 && pt0.y < height) {
			mouse.OnMouseMove(pt0.x, pt0.y);
			if (!mouse.IsInWindow()) {
				SetCapture(hWnd);
				mouse.OnMouseEnter();
			}
		} else {
			if (wParam & (MK_LBUTTON | MK_RBUTTON)) {
				mouse.OnMouseMove(pt0.x, pt0.y);
			} else {
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}
		break;
	case WM_LBUTTONDOWN:
		const POINTS pt1 = MAKEPOINTS(lParam);
		mouse.OnLBPressed(pt1.x, pt1.y);
		break;
	case WM_RBUTTONDOWN:
		const POINTS pt2 = MAKEPOINTS(lParam);
		mouse.OnRBPressed(pt2.x, pt2.y);
		break;
	case WM_LBUTTONUP:
		const POINTS pt3 = MAKEPOINTS(lParam);
		mouse.OnLBReleased(pt3.x, pt3.y);
		break;
	case WM_RBUTTONUP:
		const POINTS pt4 = MAKEPOINTS(lParam);
		mouse.OnRBReleased(pt4.x, pt4.y);
		break;
	case WM_MOUSEWHEEL:
		const POINTS pt5 = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.OnWheelDelta(pt5.x, pt5.y, delta);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Window::SetTitle(const std::wstring& title) {
	if (SetWindowText(hWnd, title.c_str()) == 0) {
		throw HWND_LAST_EXCEPT();
	}
}
#pragma endregion

#pragma region class Window::Exception
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
#pragma endregion

#pragma region class Window::HrException
Window::HrException::HrException(int line, LPCWSTR file, HRESULT hr) noexcept : Exception(line, file), hr(hr) {}

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
#pragma endregion