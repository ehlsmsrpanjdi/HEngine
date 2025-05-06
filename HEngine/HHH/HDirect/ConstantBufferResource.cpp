#include "ConstantBufferResource.h"
#include "GraphicsEngine.h"
#include "GraphicDevice.h"
#include <memory>
#include "DeviceContext.h"
//#include ""


ConstantBufferResource::ConstantBufferResource()
{
}

ConstantBufferResource::~ConstantBufferResource()
{
	if (Buffer != nullptr) {
		Buffer->Release();
	}
}


void ConstantBufferResource::CreateBuffer(std::shared_ptr<GraphicDevice> m_Device)
{
	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_DYNAMIC;
	buff_desc.ByteWidth = BufferSize;
	buff_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buff_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buff_desc.MiscFlags = 0;
	buff_desc.StructureByteStride = 0;
	HRESULT hr = m_Device->Get()->CreateBuffer(&buff_desc, nullptr, &Buffer);
	if (hr != S_OK)
	{
		assert(false);
	}

	EngineResource::SetResource(shared_from_this(), GetName());
}

void ConstantBufferResource::UpdateConstantBuffer(const void* Data, std::string_view _str)
{
	std::string str = HString::Upper(_str.data());

	std::shared_ptr<ConstantBufferResource> CRes = ConstantBufferResource::GetResource(str);
	ID3D11Buffer* Buf = CRes->GetBuffer();

	// D3D11_MAPPED_SUBRESOURCE 구조체를 선언합니다.
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	HRESULT hr = GraphicsEngine::get()->GetContext()->Get()->Map(Buf, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (hr != S_OK)
	{
		assert(false);
	}
	// 매핑된 메모리 공간에 WorldViewProj 행렬 데이터를 씁니다.
	memcpy(mappedResource.pData, Data, CRes->GetBufferSize());

	GraphicsEngine::get()->GetContext()->Get()->Unmap(Buf, 0);
}

void ConstantBufferResource::SetVSConstantBuffer(int _index, std::string_view _str)
{
	std::string str =  HString::Upper(_str.data());
	ID3D11Buffer* Buffer = ConstantBufferResource::GetResource(str)->GetBuffer();
	GraphicsEngine::get()->GetContext()->Get()->VSSetConstantBuffers(_index, 1, &Buffer);
}

void ConstantBufferResource::SetPSConstantBuffer(int _index, std::string_view _str)
{
	std::string str = HString::Upper(_str.data());
	ID3D11Buffer* Buffer = ConstantBufferResource::GetResource(str)->GetBuffer();
	GraphicsEngine::get()->GetContext()->Get()->PSSetConstantBuffers(_index, 1, &Buffer);
}

