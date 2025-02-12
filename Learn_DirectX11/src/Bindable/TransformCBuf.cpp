/**
 * @file TransformCBuf.cpp
 * @author Lhxl
 * @date 2025-2-13
 * @version build16
 */

#include "TransformCBuf.h"

TransformCBuf::TransformCBuf(Graphics& gfx, const Drawable& parent) : _vcbuf(gfx), _parent(parent) {

}

void TransformCBuf::Bind(Graphics& gfx) noexcept {
	_vcbuf.Update(gfx, DirectX::XMMatrixTranspose(_parent.GetTransformXM() * gfx.GetProjection()));
	_vcbuf.Bind(gfx);
}