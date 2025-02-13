/**
 * @file VertexShader.h
 * @author Lhxl
 * @date 2025-2-13
 * @version build17
 */

#ifndef VERTEXSHADER_H
#define VERTEXSHADER_H

#include "Bindable.h"

class VertexShader : public Bindable {
public:
	VertexShader(Graphics& gfx, const std::wstring path) noexcept(!_DEBUG);
public:
	void Bind(Graphics& gfx) noexcept override;
	ID3DBlob* GetBytecode() const noexcept;
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> _pVertexShader;
	Microsoft::WRL::ComPtr<ID3DBlob> _pBlob;
#ifndef NDEBUG
	DxgiInfoManager _infoManager;
#endif
};

#endif