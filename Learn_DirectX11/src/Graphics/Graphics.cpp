/**
 * @file ST_Timer.cpp
 * @author Lhxl
 * @date 2025-2-7
 * @version build10
 */

#pragma comment(lib, "d3d11.lib")

#include <sstream>

#include "Graphics.h"
#include "GraphicsExceptionMacro.h"

Graphics::Graphics(HWND hWnd) {
	DXGI_SWAP_CHAIN_DESC swap_desc{};
	swap_desc.BufferDesc.Width = 0;
	swap_desc.BufferDesc.Height = 0;
	swap_desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swap_desc.BufferDesc.RefreshRate.Denominator = 0;
	swap_desc.BufferDesc.RefreshRate.Numerator = 0;
	swap_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	//swap_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_desc.SampleDesc.Count = 1;
	swap_desc.SampleDesc.Quality = 0;
	swap_desc.BufferCount = 1;
	swap_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_desc.OutputWindow = hWnd;
	swap_desc.Windowed = TRUE;
	swap_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swap_desc.Flags = NULL;
	UINT swapCreateFlags = 0u;
#ifndef NDEBUG
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	HRESULT hr;
	GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE, nullptr,
		swapCreateFlags,
		nullptr, NULL, D3D11_SDK_VERSION,
		&swap_desc, &_pSwap, &_pDevice,
		nullptr,
		&_pContext
	));
	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer = nullptr;
	GFX_THROW_INFO(_pSwap->GetBuffer(NULL, __uuidof(ID3D11Resource), &pBackBuffer));
	GFX_THROW_INFO(_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &_pTarget));
}

void Graphics::EndFrame() {
	HRESULT hr;
#ifndef NDEBUG
	_infoManager.Set();
#endif
	if (FAILED(hr = _pSwap->Present(1u, 0u))) {
		if (hr == DXGI_ERROR_DEVICE_REMOVED) {
			throw GFX_DEVICE_EXCEPT(_pDevice->GetDeviceRemovedReason());
		} else {
			GFX_EXCEPT(hr);
		}
	}
}

void Graphics::ClearBuffer(float red, float green, float blue) noexcept {
	const float color[] = { red, green, blue, 1.0f };
	_pContext->ClearRenderTargetView(_pTarget.Get(), color);
}

#pragma region class Graphics::HrException
Graphics::HrException::HrException(int line, LPCWSTR file, HRESULT hr, std::vector<std::wstring> infoMsgs) noexcept : Exception(line, file), _hr(hr) {
	for (const auto& m : infoMsgs) {
		_info += m;
		_info.push_back('\n');
	}
	if (!_info.empty()) {
		_info.pop_back();
	}
}

const char* Graphics::HrException::what() const noexcept {
	std::wostringstream woss;
	woss << GetType() << std::endl
		 << L"【错误码】：0x" << std::hex << std::uppercase << GetErrorCode() << std::endl
		 << L"【错误信息】：" << GetErrorString() << std::endl;
	if (!_info.empty()) {
		woss << L"【错误描述】：" << GetErrorInfo() << std::endl << std::endl;
	}
		woss << GetOriginString();
	_whatBuffer = woss.str();
	LPCWSTR w_str = _whatBuffer.c_str();
	return lpwstr2lpcstr(w_str);
}

const char* Graphics::HrException::GetType() const noexcept {
	return "Graphics::HrException";
}

HRESULT Graphics::HrException::GetErrorCode() const noexcept {
	return _hr;
}

std::wstring Graphics::HrException::GetErrorString() const noexcept {
	wchar_t* pMsgbuf = nullptr;
	const DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		_hr,
		MAKELANGID(LANG_CHINESE_SIMPLIFIED, SUBLANG_CHINESE_SIMPLIFIED),
		reinterpret_cast<LPWSTR>(&pMsgbuf),
		0,
		nullptr);
	if (nMsgLen == 0) {
		return L"Unidentified error code";
	}
	std::wstring errorString = pMsgbuf;
	LocalFree(pMsgbuf);
	return errorString;
}

std::wstring Graphics::HrException::GetErrorInfo() const noexcept {
	return _info;
}
#pragma endregion

#pragma region class Graphics::DeviceException
const char* Graphics::DeviceException::GetType() const noexcept {
	return "Graphics::DeviceException";
}
#pragma endregion

#pragma region class Graphics::InfoException
Graphics::InfoException::InfoException(int line, LPCWSTR file, std::vector<std::wstring> infoMsgs) noexcept : Exception(line, file) {
	for (const auto& m : infoMsgs) {
		_info += m;
		_info.push_back('\n');
	}
	if (!_info.empty()) {
		_info.pop_back();
	}
}

const char* Graphics::InfoException::what() const noexcept {
	std::wostringstream woss;
	if (!_info.empty()) {
		woss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	}
	woss << GetOriginString();
	_whatBuffer = woss.str();
	LPCWSTR w_str = _whatBuffer.c_str();
	return lpwstr2lpcstr(w_str);
}

const char* Graphics::InfoException::GetType() const noexcept {
	return "ST_Graphics::InfoException";
}

std::wstring Graphics::InfoException::GetErrorInfo() const noexcept {
	return _info;
}
#pragma endregion