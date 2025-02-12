/**
 * @file IndexBuffer.h
 * @author Lhxl
 * @date 2025-2-12
 * @version build15
 */

#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include "Bindable.h"

class IndexBuffer : public Bindable{
public:
	IndexBuffer(Graphics& gfx, const std::vector<unsigned short>& indices);
	void Bind(Graphics& gfx) noexcept override;
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> _pIndexBuffer;
	D3D11_BUFFER_DESC _ibd;
	D3D11_SUBRESOURCE_DATA _isd;
};

#endif