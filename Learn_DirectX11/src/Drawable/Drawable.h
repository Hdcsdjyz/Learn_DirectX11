/**
 * @file Drawable.h
 * @author Lhxl
 * @date 2025-2-13
 * @version build17
 */

#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <memory>
#include <DirectXMath.h>

#include "../Graphics/Graphics.h"
#include "../Bindable/IndexBuffer.h"
//#include "ST_Drawable.hpp"

class Drawable {
	template<typename Type>
	friend class ST_Drawable;
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual ~Drawable() = default;
public:
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
	void Draw(Graphics& gfx) const noexcept(!_DEBUG);
	virtual void Update(float dt) noexcept = 0;
	void AddBind(std::unique_ptr<Bindable> bind) noexcept(!_DEBUG);
	void AddIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) noexcept(!_DEBUG);
private:
	virtual const std::vector<std::unique_ptr<Bindable>>& _GetStaticBinds() const noexcept = 0;
private:
	const IndexBuffer* _pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> _binds;
};

#endif