/**
 * @file DxgiInfoManager.h
 * @author Lhxl
 * @date 2025-2-7
 * @version build10
 */

#ifndef DXGIINFOMANAGER_H
#define DXGIINFOMANAGER_H

#include <vector>
#include <string>
#include <dxgidebug.h>
#include <wrl.h>

class DxgiInfoManager {
public:
	DxgiInfoManager();
	DxgiInfoManager(const DxgiInfoManager&) = delete;
	DxgiInfoManager& operator=(const DxgiInfoManager&) = delete;
	~DxgiInfoManager() = default;
public:
	void Set() noexcept;
	std::vector<std::wstring> GetMessages() const;
private:
	unsigned long long _next = 0u;
	Microsoft::WRL::ComPtr<IDXGIInfoQueue> _pDxgiInfoQueue;
};

#define GFX_THROW_NOINFO(hrcall) if (FAILED(hr = (hrcall))) throw Graphics::HrException(__LINE__, __FILEW__, hr)

#endif