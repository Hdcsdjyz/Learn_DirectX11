﻿/**
 * @file PixelShader.cpp
 * @author Lhxl
 * @date 2025-2-13
 * @version build16
 */

#pragma comment(lib, "D3DCompiler.lib")

#include <d3dcompiler.h>

#include "PixelShader.h"
#include "../Graphics/GraphicsExceptionMacro.h"

PixelShader::PixelShader(Graphics& gfx, const std::wstring path) {
	INFOMAN(gfx);
	GFX_THROW_INFO(D3DReadFileToBlob((L"./Shader/" + path).c_str(), &_pBlob));
	GFX_THROW_INFO(_GetDevice(gfx)->CreatePixelShader(_pBlob->GetBufferPointer(), _pBlob->GetBufferSize(), nullptr, &_pPixelShader));
}

void PixelShader::Bind(Graphics& gfx) noexcept {
	_GetContext(gfx)->PSSetShader(_pPixelShader.Get(), nullptr, 0u);
}