/**
 * @file VertexShader.h
 * @author Lhxl
 * @date 2025-2-12
 * @version build15
 */

#ifndef VERTEXSHADER_H
#define VERTEXSHADER_H

#include "Bindable.h"

class VertexShader : public Bindable {
public:
	VertexShader(Graphics& gfx, const std::wstring path) noexcept;
public:
	void Bind(Graphics& gfx) noexcept override;
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> _pVertexShader;
	Microsoft::WRL::ComPtr<ID3DBlob> _pBlob;
	std::wstring _path;
#ifndef NDEBUG
	DxgiInfoManager _infoManager;
#endif
};

#endif