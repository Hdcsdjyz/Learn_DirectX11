/**
 * @file TransformCBuf.h
 * @author Lhxl
 * @date 2025-2-13
 * @version build16
 */

#ifndef TRANSFORMCBUF_H
#define TRANSFORMCBUF_H

#include <DirectXMath.h>

#include "Bindable.h"
#include "ConstantBuffer.hpp"
#include "../Drawable/Drawable.h"

class TransformCBuf : public Bindable {
public:
	TransformCBuf(Graphics& gfx, const Drawable& parent);
	void Bind(Graphics& gfx) noexcept override;
private:
	VertexConstantBuffer<DirectX::XMMATRIX> _vcbuf;
	const Drawable& _parent;
};

#endif