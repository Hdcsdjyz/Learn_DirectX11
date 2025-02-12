/**
 * @file InputLayout.cpp
 * @author Lhxl
 * @date 2025-2-12
 * @version build15
 */

#include "InputLayout.h"
#include "../Graphics/GraphicsExceptionMacro.h"

InputLayout::InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& ied, ID3DBlob* pShaderBytecode) {
	INFOMAN(gfx);
	_GetDevice(gfx)->CreateInputLayout(ied.data(), (UINT)std::size(ied), pShaderBytecode->GetBufferPointer(), pShaderBytecode->GetBufferSize(), &_pInputLayout);
}

void InputLayout::Bind(Graphics& gfx) noexcept {
	_GetContext(gfx)->IASetInputLayout(_pInputLayout.Get());
}