﻿/**
 * @file Box.cpp
 * @author Lhxl
 * @date 2025-2-13
 * @version build17
 */

#include "Box.h"
#include "../Bindable/VertexBuffer.h"
#include "../Bindable/VertexShader.h"
#include "../Bindable/PixelShader.h"
#include "../Bindable/InputLayout.h"
#include "../Bindable/Topology.h"
#include "../Bindable/TransformCBuf.h"

Box::Box(
	Graphics& gfx,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist
) : _r(rdist(rng)), _roll(0.0f), _pitch(0.0f), _yaw(0.0f),
_theta(adist(rng)), _phi(adist(rng)), _chi(adist(rng)),
_droll(ddist(rng)), _dpitch(ddist(rng)), _dyaw(ddist(rng)),
_dtheta(odist(rng)), _dphi(odist(rng)), _dchi(odist(rng)) {
	if (!IsStaticInitialized()) {
		struct Vertex {
			struct {
				float x;
				float y;
				float z;
			} pos;
		};
		const std::vector<Vertex> vertices = {
			{ -1.0f, -1.0f, -1.0f },
			{ 1.0f, -1.0f, -1.0f },
			{-1.0f, 1.0f, -1.0f },
			{ 1.0f, 1.0f, -1.0f },
			{ -1.0f, -1.0f, 1.0f },
			{ 1.0f, -1.0f, 1.0f },
			{ -1.0f, 1.0f, 1.0f },
			{ 1.0f, 1.0f, 1.0f }
		};
		AddBind(std::make_unique<VertexBuffer>(gfx, vertices));
		auto pvs = std::make_unique<VertexShader>(gfx, L"VertexShader.cso");
		auto pvsbc = pvs->GetBytecode();
		AddBind(std::move(pvs));
		AddBind(std::make_unique<PixelShader>(gfx, L"PixelShader.cso"));
		const std::vector<unsigned short> indices = {
			0, 2, 1, 2, 3, 1,
			1, 3, 5, 3, 7, 5,
			2, 6, 3, 3, 6, 7,
			4, 5, 7, 4, 7, 6,
			0, 4, 2, 2, 4, 6,
			0, 1, 4, 1, 5, 4
		};
		AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));
		struct ConstantBuffer2 {
			struct {
				float r;
				float g;
				float b;
				float a;
			} face_colors[6];
		};
		const ConstantBuffer2 cb2 = {
			{
				{ 1.0f, 0.0f, 1.0f },
				{ 1.0f, 0.0f, 0.0f },
				{ 0.0f, 1.0f, 0.0f },
				{ 0.0f, 0.0f, 1.0f },
				{ 1.0f, 1.0f, 0.0f },
				{ 0.0f, 1.0f, 1.0f }
			}
		};
		AddBind(std::make_unique<PixelConstantBuffer<ConstantBuffer2>>(gfx, cb2));
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
			{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		AddBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));
		AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	} else {
		SetIndexFromStatic();
	}
	AddBind(std::make_unique<TransformCBuf>(gfx, *this));
}

void Box::Update(float dt) noexcept {
	_roll += _droll * dt;
	_pitch += _dpitch * dt;
	_yaw += _dyaw * dt;
	_theta += _dtheta * dt;
	_phi += _dphi * dt;
	_chi += _dchi * dt;
}

DirectX::XMMATRIX Box::GetTransformXM() const noexcept {
	return DirectX::XMMatrixRotationRollPitchYaw(_pitch, _yaw, _roll) *
		DirectX::XMMatrixTranslation(_r, 0.0f, 0.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(_theta, _phi, _chi) *
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
}