/**
 * @file DxgiInfoManager.cpp
 * @author Lhxl
 * @date 2025-2-7
 * @version build10
 */

#pragma comment(lib, "dxguid.lib")

#include "DxgiInfoManager.h"
#include "../Window/Window.h"

DxgiInfoManager::DxgiInfoManager() {
	typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);
	const auto hModDxgiDebug = LoadLibraryEx(L"dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (hModDxgiDebug == nullptr) {
		throw HWND_LAST_EXCEPT();
	}
	const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(reinterpret_cast<void*>(GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface")));
	if (DxgiGetDebugInterface == nullptr) {
		throw HWND_LAST_EXCEPT();
	}
	HRESULT hr;
	GFX_THROW_NOINFO(DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), &_pDxgiInfoQueue));
}

void DxgiInfoManager::Set() noexcept {
	_next = _pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::vector<std::wstring> DxgiInfoManager::GetMessages() const {
	std::vector<std::wstring> messages;
	const auto end = _pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	for (auto i = _next; i < end; i++) {
		HRESULT hr;
		SIZE_T messageLength = 0;
		GFX_THROW_NOINFO(_pDxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength));
		auto bytes = std::make_unique<byte[]>(messageLength);
		auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());
		GFX_THROW_NOINFO(_pDxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &messageLength));
		messages.emplace_back(str2lpwstr(pMessage->pDescription));
	}
	return messages;
}