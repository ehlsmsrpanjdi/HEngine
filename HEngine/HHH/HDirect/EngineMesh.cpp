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

void EngineMesh::CreateMesh(std::vector<FBXMesh*>& _AllMesh, std::shared_ptr<GraphicDevice> _Device)
{
	for (FBXMesh* mesh : _AllMesh) {
		std::shared_ptr<MH> meshinfo = std::make_shared<MH>();

		UINT Size = sizeof(mesh->vertices[0]);
		UINT ArraySize = mesh->vertices.size();

		meshinfo->BufferSize = Size;
		meshinfo->Vertex = CreateBuffer(ArraySize, Size, (UINT*)mesh->vertices.data(), mesh->MeshName, _Device);

		Size = sizeof(mesh->indices[0]);
		ArraySize = mesh->indices.size();

		meshinfo->IndexBufferSize = ArraySize;
		meshinfo->Index = CreateIndexBuffer(ArraySize, Size, mesh->indices.data(), mesh->MeshName, _Device);
		 
		MeshMap.insert(std::make_pair(mesh->MeshName, meshinfo));
	}
}

MH* EngineMesh::GetMesh(std::string_view _str)
{
	std::string str = HString::Upper(_str.data());
	if (MeshMap.contains(str) == false) {
		assert(false);
	}
	return MeshMap[str].get();
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

