/**
 * @file InputLayout.h
 * @author Lhxl
 * @date 2025-2-12
 * @version build15
 */

#ifndef INPUTLAYOUT_H
#define INPUTLAYOUT_H

#include "Bindable.h"

class InputLayout :public Bindable {
public:
	InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& ied, ID3DBlob* pShaderBytecode);
public:
	void Bind(Graphics& gfx) noexcept override;
private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> _pInputLayout;
};

#endif