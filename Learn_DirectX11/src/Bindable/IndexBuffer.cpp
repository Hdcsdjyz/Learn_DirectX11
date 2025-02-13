/**
 * @file IndexBuffer.cpp
 * @author Lhxl
 * @date 2025-2-13
 * @version build17
 */

#include "IndexBuffer.h"
#include "../Graphics/GraphicsExceptionMacro.h"

IndexBuffer::IndexBuffer(Graphics& gfx, const std::vector<unsigned short>& indices) : _count(indices.size()) {
	INFOMAN(gfx);
	_ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	_ibd.Usage = D3D11_USAGE_DEFAULT;
	_ibd.CPUAccessFlags = 0u;
	_ibd.MiscFlags = 0u;
	_ibd.ByteWidth = (UINT)(_count * sizeof(unsigned short));
	_ibd.StructureByteStride = sizeof(unsigned short);
	_isd.pSysMem = indices.data();
	GFX_THROW_INFO(_GetDevice(gfx)->CreateBuffer(&_ibd, &_isd, &_pIndexBuffer));
}

void IndexBuffer::Bind(Graphics& gfx) noexcept {
	_GetContext(gfx)->IASetIndexBuffer(_pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

UINT IndexBuffer::GetCount() const noexcept {
	return _count;
}