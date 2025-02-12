/**
 * @file Bindable.h
 * @author Lhxl
 * @date 2025-2-12
 * @version build15
 */

#ifndef BINDABLE_H
#define BINDABLE_H

#include "../Graphics/Graphics.h"

class Bindable {
public:
	virtual ~Bindable() = default;
public:
	virtual void Bind(Graphics& gfx) noexcept = 0;
protected:
	static ID3D11DeviceContext* _GetContext(Graphics& gfx) noexcept;
	static ID3D11Device* _GetDevice(Graphics& gfx) noexcept;
	static DxgiInfoManager& _GetInfoManager(Graphics& gfx) noexcept(!_DEBUG);
};

#endif