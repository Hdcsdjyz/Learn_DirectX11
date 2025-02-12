/**
 * @file VertexBuffer.cpp
 * @author Lhxl
 * @date 2025-2-12
 * @version build15
 */

#include "VertexBuffer.h"

void VertexBuffer::Bind(Graphics& gfx) noexcept {
	_GetContext(gfx)->IASetVertexBuffers(0u, 1u, _pVertexBuffer.GetAddressOf(), &_stride, &_offset);
}