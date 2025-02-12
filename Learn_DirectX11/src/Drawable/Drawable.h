/**
 * @file Drawable.h
 * @author Lhxl
 * @date 2025-2-13
 * @version build16
 */

#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <memory>
#include <DirectXMath.h>

#include "../Graphics/Graphics.h"
#include "../Bindable/IndexBuffer.h"

class Drawable {
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual ~Drawable() = default;
public:
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
	void Draw(Graphics& gfx) const noexcept(!_DEBUG);
	virtual void Update(float dt) noexcept = 0;
	void AddBind(std::unique_ptr<Bindable> bind) noexcept(!_DEBUG);
	void AddIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) noexcept;
private:
	const IndexBuffer* _pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> _binds;
};

#endif