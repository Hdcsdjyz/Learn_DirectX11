/**
 * @file Drawable.cpp
 * @author Lhxl
 * @date 2025-2-13
 * @version build17
 */

#include "Drawable.h"

void Drawable::Draw(Graphics& gfx) const noexcept(!_DEBUG) {
	for (auto& b : _binds) {
		b->Bind(gfx);
	}
	for (auto& b : _GetStaticBinds()) {
		b->Bind(gfx);
	}
	gfx.DrawIndexed(_pIndexBuffer->GetCount());
}

void Drawable::AddBind(std::unique_ptr<Bindable> bind) noexcept(!_DEBUG) {
	assert("必须使用 函数AddIndexBuffer 来绑定索引缓存。" && typeid(*bind) != typeid(Bindable));
	_binds.push_back(std::move(bind));
}

void Drawable::AddIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) noexcept(!_DEBUG) {
	assert("正在尝试重复添加索引缓存" && _pIndexBuffer == nullptr);
	_pIndexBuffer = ibuf.get();
	_binds.push_back(std::move(ibuf));
}