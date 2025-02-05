/**
 * @file ST_Timer.cpp
 * @author Lhxl
 * @date 2025-2-6
 * @version build8
 */

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "../Window/ST_Window.h"
#include <d3d11_1.h>

class Graphics {
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();
public:
	void EndFrame();
	void ClearBuffer(float red, float green, float blue) noexcept;
private:
	ID3D11Device* _pDevice = nullptr;
	IDXGISwapChain* _pSwap = nullptr;
	ID3D11DeviceContext* _pContext = nullptr;
	ID3D11RenderTargetView* _pTarget = nullptr;
};

#endif