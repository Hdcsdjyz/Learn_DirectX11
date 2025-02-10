/**
 * @file ST_Timer.cpp
 * @author Lhxl
 * @date 2025-2-10
 * @version build11
 */

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#include <sstream>
#include <d3dcompiler.h>

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

void Graphics::DrawTestTriangle() {
	struct Vertex {
		float x;
		float y;
	};
	const Vertex vertices[] = {
		{ 0.0f, 0.5f },
		{ 0.5f, -0.5f },
		{ -0.5f, -0.5f }
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(vertices);
	bd.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;
	HRESULT hr;
	GFX_THROW_INFO(_pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));
	UINT stride = sizeof(Vertex);
	UINT offset = 0u;
	_pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

	// PixelShader
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	GFX_THROW_INFO(D3DReadFileToBlob(L"./Shader/PixelShader.cso", &pBlob));
	GFX_THROW_INFO(_pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));
	_pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

	// VertexShader
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	GFX_THROW_INFO(D3DReadFileToBlob(L"./Shader/VertexShader.cso", &pBlob));
	GFX_THROW_INFO(_pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));
	_pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

	// 顶点输入布局
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{ "Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	GFX_THROW_INFO(_pDevice->CreateInputLayout(ied, (UINT)std::size(ied), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout));
	_pContext->IASetInputLayout(pInputLayout.Get());

	// 绑定渲染目标
	_pContext->OMSetRenderTargets(1u, _pTarget.GetAddressOf(), nullptr);

	// 设置几何
	_pContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 设置视窗
	D3D11_VIEWPORT vp = {};
	vp.Width = 1280;
	vp.Height = 720;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	_pContext->RSSetViewports(1u, &vp);

	GFX_THROW_INFO_ONLY(_pContext->Draw((UINT)std::size(vertices), 0u));
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
	if (!GetOriginString().empty()) {
		woss << GetOriginString();
	}
		
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
	woss << GetType() << std::endl;
	if (!_info.empty()) {
		woss << L"\n【错误信息】\n" << GetErrorInfo() << std::endl << std::endl;
	}
	// if (!GetOriginString().empty()) {
	// 	woss << L"\n【错误详情】\n" << GetOriginString();
	// }
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