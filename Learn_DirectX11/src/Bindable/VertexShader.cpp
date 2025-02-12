/**
 * @file VertexShader.cpp
 * @author Lhxl
 * @date 2025-2-12
 * @version build15
 */

#pragma comment(lib, "D3DCompiler.lib")

#include <d3dcompiler.h>

#include "VertexShader.h"
#include "../Graphics/GraphicsExceptionMacro.h"

VertexShader::VertexShader(Graphics& gfx, const std::wstring path) noexcept : _path(path) {
	INFOMAN(gfx);
	GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &_pBlob));
	GFX_THROW_INFO(_GetDevice(gfx)->CreateVertexShader(_pBlob->GetBufferPointer(), _pBlob->GetBufferSize(), nullptr, &_pVertexShader));
}

void VertexShader::Bind(Graphics& gfx) noexcept {
	_GetContext(gfx)->VSSetShader(_pVertexShader.Get(), nullptr, 0u);
}