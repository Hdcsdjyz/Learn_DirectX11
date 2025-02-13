/**
 * @file ST_Drawable.h
 * @author Lhxl
 * @date 2025-2-13
 * @version build17
 */

#ifndef ST_DRAWABLE_H
#define ST_DRAWABLE_H

#include "Drawable.h"

template<typename Type>
class ST_Drawable : public Drawable {
public:
	bool IsStaticInitialized() const noexcept {
		return !_staticBinds.empty();
	}

	void AddStaticBind(std::unique_ptr<Bindable> bind) noexcept(!_DEBUG) {
		assert("必须使用 函数AddIndexBuffer 来绑定索引缓存。" && typeid(*bind) != typeid(Bindable));
		_staticBinds.push_back(std::move(bind));
	}

	void AddStaticIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) noexcept(!_DEBUG) {
		assert("正在尝试重复添加索引缓存" && _pIndexBuffer == nullptr);
		_pIndexBuffer = ibuf.get();
		_staticBinds.push_back(std::move(ibuf));
	}

	void SetIndexFromStatic() noexcept(!_DEBUG) {
		assert("正在尝试重复添加索引缓存" && _pIndexBuffer == nullptr);
		for (const auto& b : _staticBinds) {
			if (const auto p = dynamic_cast<IndexBuffer*>(b.get())) {
				_pIndexBuffer = p;
				return;
			}
		}
		assert("添加索引缓存失败" && _pIndexBuffer == nullptr);
	}
private:
	const std::vector<std::unique_ptr<Bindable>>& _GetStaticBinds() const noexcept override {
		return _staticBinds;
	}
private:
	static std::vector<std::unique_ptr<Bindable>> _staticBinds;
};

template<typename Type>
std::vector<std::unique_ptr<Bindable>> ST_Drawable<Type>::_staticBinds;

#endif