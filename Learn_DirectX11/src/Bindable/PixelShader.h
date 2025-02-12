/**
 * @file PixelShader.h
 * @author Lhxl
 * @date 2025-2-12
 * @version build15
 */

#ifndef PIXELSHADER_H
#define PIXELSHADER_H

#include "Bindable.h"

class PixelShader : public Bindable {
public:
	PixelShader(Graphics& gfx, const std::wstring path);
public:
	void Bind(Graphics& gfx) noexcept override;
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> _pPixelShader;
	Microsoft::WRL::ComPtr<ID3DBlob> _pBlob;
};

#endif