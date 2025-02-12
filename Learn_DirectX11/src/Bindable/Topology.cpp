/**
 * @file Topology.cpp
 * @author Lhxl
 * @date 2025-2-13
 * @version build16
 */

#include "Topology.h"
#include "../Graphics/GraphicsExceptionMacro.h"

Topology::Topology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type) : _type(type) {
	INFOMAN(gfx);
}

void Topology::Bind(Graphics& gfx) noexcept {
	_GetContext(gfx)->IASetPrimitiveTopology(_type);
}