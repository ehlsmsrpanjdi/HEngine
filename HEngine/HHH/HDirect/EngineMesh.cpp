#include "EngineMesh.h"
#include "EngineHelper/AllStruct.h"
#include <cassert>
#include "GraphicDevice.h"
#include "EngineHelper/HString.h"


EngineMesh::EngineMesh() 
{
}

EngineMesh::~EngineMesh() 
{
	MeshMap.clear();
}

void EngineMesh::CreateMesh(std::vector<std::shared_ptr<FMesh>>& _AllMesh, std::shared_ptr<GraphicDevice> _Device)
{
	for (std::shared_ptr<FMesh> mesh : _AllMesh) {
		std::shared_ptr<MH> meshinfo = std::make_shared<MH>();

		UINT Size = sizeof(mesh->vertices[0]);
		UINT ArraySize = static_cast<UINT>(mesh->vertices.size());

		meshinfo->BufferSize = Size;
		meshinfo->Vertex = CreateBuffer(ArraySize, Size, (UINT*)mesh->vertices.data(), mesh->MeshName, _Device);

		Size = sizeof(mesh->indices[0]);
		ArraySize = static_cast<UINT>(mesh->indices.size());

		meshinfo->IndexBufferSize = ArraySize;
		meshinfo->Index = CreateIndexBuffer(ArraySize, Size, mesh->indices.data(), mesh->MeshName, _Device);
		 
		MeshMap.insert(std::make_pair(mesh->MeshName, meshinfo));
	}

	Test(_Device);
}

MH* EngineMesh::GetMesh(std::string_view _str)
{
	std::string str = HString::Upper(_str.data());
	if (MeshMap.contains(str) == false) {
		assert(false);
	}
	return MeshMap[str].get();
}

struct Vertex {
	DirectX::XMFLOAT3 position; // ��ġ�� (x, y, z)
	DirectX::XMFLOAT2 uv;      // �ؽ�ó ��ǥ (u, v)
};

void EngineMesh::Test(std::shared_ptr<GraphicDevice> _Device)
{
	std::vector<Vertex> vertexBuffer = {
		{DirectX::XMFLOAT3(-1.0f,1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f)}, // ���� 0
		{DirectX::XMFLOAT3(1.0f,1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f)}, // ���� 1
		{DirectX::XMFLOAT3(1.0f,-1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f)}, // ���� 2
		{DirectX::XMFLOAT3(-1.0f,-1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f)}, // ���� 3
	};


		ID3D11Buffer* vbuffer = nullptr;
	{
		D3D11_BUFFER_DESC buff_desc = {};
		buff_desc.Usage = D3D11_USAGE_DEFAULT;
		buff_desc.ByteWidth = static_cast<UINT>(vertexBuffer.size()) * sizeof(DirectX::XMFLOAT3);
		buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buff_desc.CPUAccessFlags = 0;
		buff_desc.MiscFlags = 0;


		D3D11_SUBRESOURCE_DATA init_data = {};
		init_data.pSysMem = vertexBuffer.data();

		HRESULT hr = _Device->Get()->CreateBuffer(&buff_desc, &init_data, &vbuffer);

		if (hr != S_OK)
		{
			assert(false);
		}
	}

	// �ε��� ���� ������
	std::vector<UINT> indexBuffer = {
		0,1,3,
		3,1,2
	};

	ID3D11Buffer* ibuffer = nullptr;
	{

		D3D11_BUFFER_DESC buff_desc = {};
		buff_desc.Usage = D3D11_USAGE_IMMUTABLE;
		buff_desc.ByteWidth = static_cast<UINT>(indexBuffer.size()) * sizeof(UINT);
		buff_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		buff_desc.CPUAccessFlags = 0;
		buff_desc.MiscFlags = 0;
		buff_desc.StructureByteStride = 0;



		D3D11_SUBRESOURCE_DATA init_data = {};
		init_data.pSysMem = indexBuffer.data();

		HRESULT hr = _Device->Get()->CreateBuffer(&buff_desc, &init_data, &ibuffer);
		if (hr != S_OK)
		{
			assert(false);
		}
	}

	MH* meshinfo = new MH();
	meshinfo->Index = ibuffer;
	meshinfo->IndexBufferSize = static_cast<UINT>(indexBuffer.size());
	meshinfo->Vertex = vbuffer;
	meshinfo->BufferSize = sizeof(DirectX::XMFLOAT3);

	MeshMap.insert(std::make_pair("TEST", std::shared_ptr<MH>(meshinfo)));
}

ID3D11Buffer* EngineMesh::CreateBuffer(UINT _ArraySize, UINT _Size, UINT* _List, std::string _str, std::shared_ptr<GraphicDevice> _Device)
{
	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_DEFAULT;
	buff_desc.ByteWidth = _ArraySize * _Size;
	buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;

	ID3D11Buffer* buffer = nullptr;

	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = _List;

	HRESULT hr = _Device->Get()->CreateBuffer(&buff_desc, &init_data, &buffer);

	if (hr != S_OK)
	{
		assert(false);
	}
	return buffer;

}

ID3D11Buffer* EngineMesh::CreateIndexBuffer(UINT _ArraySize, UINT _Size, UINT* _List, std::string _str, std::shared_ptr<GraphicDevice> _Device)
{
	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_IMMUTABLE;
	buff_desc.ByteWidth = _ArraySize * _Size;
	buff_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;
	buff_desc.StructureByteStride = 0;

	ID3D11Buffer* buffer = nullptr;


	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = _List;

	HRESULT hr = _Device->Get()->CreateBuffer(&buff_desc, &init_data, &buffer);
	if (hr != S_OK)
	{
		assert(false);
	}
	return buffer;
}

