/**
 * @file Bindable.cpp
 * @author Lhxl
 * @date 2025-2-12
 * @version build15
 */

#include "Bindable.h"

ID3D11DeviceContext* Bindable::_GetContext(Graphics& gfx) noexcept {
	return gfx._pContext.Get();
}

ID3D11Device* Bindable::_GetDevice(Graphics& gfx) noexcept {
	return gfx._pDevice.Get();
}

DxgiInfoManager& Bindable::_GetInfoManager(Graphics& gfx) noexcept(!_DEBUG) {
	return gfx._infoManager;
}