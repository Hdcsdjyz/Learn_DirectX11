/**
 * @file VertexBuffer.h
 * @author Lhxl
 * @date 2025-2-12
 * @version build15
 */

#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "Bindable.h"
#include "../Graphics/GraphicsExceptionMacro.h"

class VertexBuffer : public Bindable {
public:
	template<typename Type>
	VertexBuffer(Graphics& gfx, const std::vector<Type>& vertices) : _stride(sizeof(Type)), offset(0u) {
		_bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		_bd.Usage = D3D11_USAGE_DEFAULT;
		_bd.CPUAccessFlags = 0u;
		_bd.MiscFlags = 0u;
		_bd.ByteWidth = sizeof(Type) * vertices.size();
		_bd.StructureByteStride = _stride;
		_sd.pSysMem = vertices.data();
		GFX_THROW_INFO(_GetDevice(gfx)->CreateBuffer(&_bd, &_sd, &_pVertexBuffer));
	}
	void Bind(Graphics& gfx) noexcept override;
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> _pVertexBuffer;
	D3D11_BUFFER_DESC _bd;
	D3D11_SUBRESOURCE_DATA _sd;
	UINT _stride;
	UINT _offset;
};

#endif