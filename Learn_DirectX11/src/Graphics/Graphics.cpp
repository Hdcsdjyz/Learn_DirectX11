/**
 * @file ST_Timer.cpp
 * @author Lhxl
 * @date 2025-2-6
 * @version build8
 */

#pragma comment(lib, "d3d11.lib")

#include "Graphics.h"

Graphics::Graphics(HWND hWnd) {
	DXGI_SWAP_CHAIN_DESC swap_desc{};
	swap_desc.BufferDesc.Width = 0;
	swap_desc.BufferDesc.Height = 0;
	swap_desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swap_desc.BufferDesc.RefreshRate.Denominator = 0;
	swap_desc.BufferDesc.RefreshRate.Numerator = 0;
	swap_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swap_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_desc.SampleDesc.Count = 1;
	swap_desc.SampleDesc.Quality = 0;
	swap_desc.BufferCount = 1;
	swap_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_desc.OutputWindow = hWnd;
	swap_desc.Windowed = TRUE;
	swap_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swap_desc.Flags = NULL;
	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE, nullptr,
		NULL,
		nullptr, NULL, D3D11_SDK_VERSION,
		&swap_desc, &_pSwap, &_pDevice,
		nullptr,
		&_pContext
	);
	ID3D11Resource* pBackBuffer = nullptr;
	_pSwap->GetBuffer(NULL, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer));
	_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &_pTarget);
}

Graphics::~Graphics() {
	if (_pContext != nullptr) {
		_pContext->Release();
	}
	if (_pSwap != nullptr) {
		_pSwap->Release();
	}
	if (_pDevice != nullptr) {
		_pDevice->Release();
	}
	if (_pTarget != nullptr) {
		_pTarget->Release();
	}
}

void Graphics::EndFrame() {
	_pSwap->Present(1u, 0u);
}

void Graphics::ClearBuffer(float red, float green, float blue) noexcept {
	const float color[] = { red, green, blue, 1.0f };
	_pContext->ClearRenderTargetView(_pTarget, color);
}