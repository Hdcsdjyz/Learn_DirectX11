/**
 * @file Graphics.cpp
 * @author Lhxl
 * @date 2025-2-12
 * @version build14
 */

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#include <sstream>
#include <d3dcompiler.h>
#include <DirectXMath.h>

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
	swap_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
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
	
	// 深度缓冲
	D3D11_DEPTH_STENCIL_DESC dsd = {};
	dsd.DepthEnable = TRUE;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsd.DepthFunc = D3D11_COMPARISON_LESS;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDSState;
	GFX_THROW_INFO(_pDevice->CreateDepthStencilState(&dsd, &pDSState));
	_pContext->OMSetDepthStencilState(pDSState.Get(), 1u);
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = 1280u;
	descDepth.Height = 720u;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.CPUAccessFlags = 0u;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	GFX_THROW_INFO(_pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));
	D3D11_DEPTH_STENCIL_VIEW_DESC DSVdesc = {};
	DSVdesc.Format = DXGI_FORMAT_D32_FLOAT;
	DSVdesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DSVdesc.Texture2D.MipSlice = 0u;
	GFX_THROW_INFO(_pDevice->CreateDepthStencilView(pDepthStencil.Get(), &DSVdesc, &_pDSV));
	_pContext->OMSetRenderTargets(1u, _pTarget.GetAddressOf(), _pDSV.Get());
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
	_pContext->ClearDepthStencilView(_pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Graphics::DrawTestTriangle(float angle, float x, float z) {
	struct Vertex {
		struct {
			float x;
			float y;
			float z;
		} pos;
	};
	const Vertex vertices[] = {
		{ -1.0f, -1.0f, -1.0f },
		{ 1.0f, -1.0f, -1.0f },
		{ -1.0f, 1.0f, -1.0f },
		{ 1.0f, 1.0f, -1.0f },
		{ -1.0f, -1.0f, 1.0f },
		{ 1.0f, -1.0f, 1.0f },
		{ -1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f }
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

	// 顺序缓存
	const unsigned short indices[]{
		0, 2, 1, 2, 3, 1,
		1, 3, 5, 3, 7, 5,
		2, 6, 3, 3, 6, 7,
		4, 5, 7, 4, 7, 6,
		0, 4, 2, 2, 4, 6,
		0, 1, 4, 1, 5, 4
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = sizeof(indices);
	ibd.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices;
	GFX_THROW_INFO(_pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer));
	_pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

	// 常数缓存
	struct ConstantBuffer {
		DirectX::XMMATRIX transform;
	};
	const ConstantBuffer cb = {
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixRotationZ(angle) *
			DirectX::XMMatrixRotationX(angle) *
			DirectX::XMMatrixTranslation(x, 0.0f, z + 8.0f) *
			DirectX::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 1.0f, 10.0f)
		)
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
	D3D11_BUFFER_DESC cbd = {};
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(cb);
	cbd.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &cb;
	GFX_THROW_INFO(_pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer));
	_pContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());

	struct ConstantBuffer2 {
		struct {
			float r;
			float g;
			float b;
			float a;
		}face_colors[6];
	};

	const ConstantBuffer2 cb2 = {
		{
			{ 1.0f, 0.0f, 1.0f },
			{ 1.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f },
			{ 1.0f, 1.0f, 0.0f },
			{ 0.0f, 1.0f, 1.0f }
		}
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer2;
	D3D11_BUFFER_DESC cbd2 = {};
	cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd2.Usage = D3D11_USAGE_DEFAULT;
	cbd2.CPUAccessFlags = 0u;
	cbd2.MiscFlags = 0u;
	cbd2.ByteWidth = sizeof(cb2);
	cbd2.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd2 = {};
	csd2.pSysMem = &cb2;
	GFX_THROW_INFO(_pDevice->CreateBuffer(&cbd2, &csd2, &pConstantBuffer2));
	_pContext->PSSetConstantBuffers(0u, 1u, pConstantBuffer2.GetAddressOf());

	// 像素着色器
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	GFX_THROW_INFO(D3DReadFileToBlob(L"./Shader/PixelShader.cso", &pBlob));
	GFX_THROW_INFO(_pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));
	_pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

	// 顶点着色器
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	GFX_THROW_INFO(D3DReadFileToBlob(L"./Shader/VertexShader.cso", &pBlob));
	GFX_THROW_INFO(_pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));
	_pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

	// 顶点输入布局
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	GFX_THROW_INFO(_pDevice->CreateInputLayout(ied, (UINT)std::size(ied), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout));
	_pContext->IASetInputLayout(pInputLayout.Get());

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

	GFX_THROW_INFO_ONLY(_pContext->DrawIndexed((UINT)std::size(indices), 0u, 0u));
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