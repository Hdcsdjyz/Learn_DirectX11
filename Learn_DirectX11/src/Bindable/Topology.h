/**
 * @file Topology.h
 * @author Lhxl
 * @date 2025-2-12
 * @version build15
 */

#ifndef TOPOLOGY_H
#define TOPOLOGY_H

#include "Bindable.h"

class Topology : public Bindable {
public:
	Topology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type);
public:
	void Bind(Graphics& gfx) noexcept override;
protected:
	D3D11_PRIMITIVE_TOPOLOGY _type;
};

#endif