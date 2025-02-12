/**
 * @file Box.h
 * @author Lhxl
 * @date 2025-2-13
 * @version build16
 */

#ifndef BOX_H
#define BOX_H

#include <random>

#include "Drawable.h"

class Box : public Drawable {
public:
	Box(
		Graphics& gfx,
		std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist
	);
public:
	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	float _r;
	float _roll;
	float _pitch;
	float _yaw;
	float _theta;
	float _phi;
	float _chi;
	float _droll;
	float _dpitch;
	float _dyaw;
	float _dtheta;
	float _dphi;
	float _dchi;
};

#endif