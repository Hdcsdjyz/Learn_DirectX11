/**
 * @file ConstantBuffer.h
 * @author Lhxl
 * @date 2025-2-13
 * @version build16
 */

#ifndef CONSTANTBUFFER_H
#define CONSTANTBUFFER_H

#include "Bindable.h"
#include "../Graphics/GraphicsExceptionMacro.h"

template<typename Type>
class ConstantBuffer : public Bindable{
public:
	ConstantBuffer(Graphics& gfx) {
		INFOMAN(gfx);
		_cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		_cbd.Usage = D3D11_USAGE_DYNAMIC;
		_cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		_cbd.MiscFlags = 0u;
		_cbd.ByteWidth = sizeof(Type);
		_cbd.StructureByteStride = 0u;
		_csd.pSysMem = nullptr;
		GFX_THROW_INFO(_GetDevice(gfx)->CreateBuffer(&_cbd, nullptr, &_pConstantBuffer));
	}
	ConstantBuffer(Graphics& gfx, const Type& consts) {
		INFOMAN(gfx);
		_cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		_cbd.Usage = D3D11_USAGE_DYNAMIC;
		_cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		_cbd.MiscFlags = 0u;
		_cbd.ByteWidth = sizeof(consts);
		_cbd.StructureByteStride = 0u;
		_csd.pSysMem = &consts;
		GFX_THROW_INFO(_GetDevice(gfx)->CreateBuffer(&_cbd, &_csd, &_pConstantBuffer));
	}
public:
	void Update(Graphics& gfx, const Type& consts) {
		INFOMAN(gfx);
		D3D11_MAPPED_SUBRESOURCE msr;
		GFX_THROW_INFO(_GetContext(gfx)->Map(_pConstantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr));
		memcpy(msr.pData, &consts, sizeof(consts));
		_GetContext(gfx)->Unmap(_pConstantBuffer.Get(), 0u);
	}
protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> _pConstantBuffer;
	D3D11_BUFFER_DESC _cbd = {};
	D3D11_SUBRESOURCE_DATA _csd = {};
};

template<typename Type>
class VertexConstantBuffer : public ConstantBuffer<Type> {
	using ConstantBuffer<Type>::_pConstantBuffer;
	using Bindable::_GetContext;
public:
	using ConstantBuffer<Type>::ConstantBuffer;
public:
	void Bind(Graphics& gfx) noexcept override {
		_GetContext(gfx)->VSSetConstantBuffers(0u, 1u, _pConstantBuffer.GetAddressOf());
	}
};

template<typename Type>
class  PixelConstantBuffer : public ConstantBuffer<Type> {
	using ConstantBuffer<Type>::_pConstantBuffer;
	using Bindable::_GetContext;
public:
	using ConstantBuffer<Type>::ConstantBuffer;
public:
	void Bind(Graphics& gfx) noexcept override {
		_GetContext(gfx)->PSSetConstantBuffers(0u, 1u, _pConstantBuffer.GetAddressOf());
	}
};

#endif