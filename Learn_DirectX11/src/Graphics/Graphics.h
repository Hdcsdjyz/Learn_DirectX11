/**
 * @file Graphics.h
 * @author Lhxl
 * @date 2025-2-11
 * @version build13
 */

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <vector>
#include <wrl.h>

#include "../ST_General/ST_Window.h"
#include "../ST_General/ST_Exception.h"
#include "DxgiInfoManager.h"

class Graphics {
public:
	class Exception : public ST_Exception {
		using ST_Exception::ST_Exception;
	};
public:
	class HrException : public Exception {
	public:
		HrException(int line, LPCWSTR file, HRESULT hr, std::vector<std::wstring> infoMsgs = {}) noexcept;
	public:
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::wstring GetErrorString() const noexcept;
		std::wstring GetErrorInfo() const noexcept;
	private:
		HRESULT _hr;
		std::wstring _info;
	};
public:
	class DeviceException : public HrException {
		using HrException::HrException;
	public:
		const char* GetType() const noexcept override;
	private:
		std::wstring _reason;
	};
public:
	class InfoException : public Exception {
	public:
		InfoException(int line, LPCWSTR file, std::vector<std::wstring> infoMsgs = {}) noexcept;
	public:
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		std::wstring GetErrorInfo() const noexcept;
	private:
		std::wstring _info;
	};
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics() = default;
public:
	void EndFrame();
	void ClearBuffer(float red, float green, float blue) noexcept;
	void DrawTestTriangle(float angle);
private:
	Microsoft::WRL::ComPtr<ID3D11Device> _pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> _pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> _pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _pTarget;
#ifndef NDEBUG
	DxgiInfoManager _infoManager;
#endif
};

#endif