#include "EngineScene.h"
#include "EngineHelper/AllStruct.h"
#include <cassert>
#include "GraphicDevice.h"
#include "EngineHelper/HString.h"
#include "EngineHelper/EngineFScene.h"
#include "EngineHelper/EngineFMesh.h"


EngineScene::EngineScene() 
{
}

EngineScene::~EngineScene() 
{
	AllScene.clear();
}

void EngineScene::CreateMesh(std::vector<std::shared_ptr<EngineFScene>> _Scenes, std::shared_ptr<class GraphicDevice> _Device)
{
	for (std::shared_ptr<EngineFScene> scene : _Scenes) {
		std::shared_ptr<FScene> fscene = std::make_shared<FScene>();
		for (std::pair<std::string, std::shared_ptr<EngineFMesh>> meshinfo : scene->MeshMap) {
			UINT Size = meshinfo.second->GetSize();
			UINT arraysize = static_cast<UINT>(meshinfo.second->GetArraySize());

			std::shared_ptr<MH> Mesh = std::make_shared<MH>();
			Mesh->BufferSize = Size;
			Mesh->Vertex = CreateBuffer(arraysize, Size, (FBuffer*)meshinfo.second->GetVertex().data(), _Device);

			Size = meshinfo.second->GetIndexSize();
			arraysize = static_cast<UINT>(meshinfo.second->GetIndexArraySize());
			Mesh->IndexBufferSize = arraysize;
			Mesh->Index = CreateIndexBuffer(arraysize, Size, (UINT*)meshinfo.second->GetIndices().data(), _Device);

			Mesh->TextureName = meshinfo.second->GetTextureName();

			fscene->Meshs[meshinfo.first] = Mesh;
		}
		AllScene[scene->SceneName] = fscene;
	}

	//Test(_Device);
}

std::unordered_map<std::string, std::shared_ptr<MH>>& EngineScene::GetMesh(std::string_view _str)
{
	std::string str = HString::Upper(_str.data());

	if (AllScene.contains(str) == false) {
		assert(false);
	}
	return AllScene[str]->Meshs;
}

//#pragma region "테스트용"
//struct Vertex {
//	DirectX::XMFLOAT3 position; // 위치값 (x, y, z)
//	DirectX::XMFLOAT2 uv;      // 텍스처 좌표 (u, v)
//};
//
//void EngineScene::Test(std::shared_ptr<GraphicDevice> _Device)
//{
//	DirectX::XMFLOAT3 position_list[] =
//	{
//		{ DirectX::XMFLOAT3(-0.5f,-0.5f,-0.5f)},
//		{ DirectX::XMFLOAT3(-0.5f,0.5f,-0.5f) },
//		{ DirectX::XMFLOAT3(0.5f,0.5f,-0.5f) },
//		{ DirectX::XMFLOAT3(0.5f,-0.5f,-0.5f)},
//
//		//BACK FACE
//		{ DirectX::XMFLOAT3(0.5f,-0.5f,0.5f) },
//		{ DirectX::XMFLOAT3(0.5f,0.5f,0.5f) },
//		{ DirectX::XMFLOAT3(-0.5f,0.5f,0.5f)},
//		{ DirectX::XMFLOAT3(-0.5f,-0.5f,0.5f) }
//	};
//
//	DirectX::XMFLOAT2 texcoord_list[] =
//	{
//		{ DirectX::XMFLOAT2(0.0f,0.0f) },
//		{ DirectX::XMFLOAT2(0.0f,1.0f) },
//		{ DirectX::XMFLOAT2(1.0f,0.0f) },
//		{ DirectX::XMFLOAT2(1.0f,1.0f) }
//	};
//
//
//
//	Vertex vertex_list[] =
//	{
//		//X - Y - Z
//		//FRONT FACE
//		{ position_list[0],texcoord_list[1] },
//		{ position_list[1],texcoord_list[0] },
//		{ position_list[2],texcoord_list[2] },
//		{ position_list[3],texcoord_list[3] },
//
//
//		{ position_list[4],texcoord_list[1] },
//		{ position_list[5],texcoord_list[0] },
//		{ position_list[6],texcoord_list[2] },
//		{ position_list[7],texcoord_list[3] },
//
//
//		{ position_list[1],texcoord_list[1] },
//		{ position_list[6],texcoord_list[0] },
//		{ position_list[5],texcoord_list[2] },
//		{ position_list[2],texcoord_list[3] },
//
//		{ position_list[7],texcoord_list[1] },
//		{ position_list[0],texcoord_list[0] },
//		{ position_list[3],texcoord_list[2] },
//		{ position_list[4],texcoord_list[3] },
//
//		{ position_list[3],texcoord_list[1] },
//		{ position_list[2],texcoord_list[0] },
//		{ position_list[5],texcoord_list[2] },
//		{ position_list[4],texcoord_list[3] },
//
//		{ position_list[7],texcoord_list[1] },
//		{ position_list[6],texcoord_list[0] },
//		{ position_list[1],texcoord_list[2] },
//		{ position_list[0],texcoord_list[3] }
//	};
//
//	
//
//
//	UINT size_list = ARRAYSIZE(vertex_list);
//
//
//
//
//		ID3D11Buffer* vbuffer = nullptr;
//	{
//		D3D11_BUFFER_DESC buff_desc = {};
//		buff_desc.Usage = D3D11_USAGE_DEFAULT;
//		buff_desc.ByteWidth = size_list * sizeof(Vertex);
//		buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//		buff_desc.CPUAccessFlags = 0;
//		buff_desc.MiscFlags = 0;
//
//
//		D3D11_SUBRESOURCE_DATA init_data = {};
//		init_data.pSysMem = vertex_list;
//
//		HRESULT hr = _Device->Get()->CreateBuffer(&buff_desc, &init_data, &vbuffer);
//
//		if (hr != S_OK)
//		{
//			assert(false);
//		}
//	}
//
//
//	unsigned int index_list[] =
//	{
//		//FRONT SIDE
//		0,1,2,  //FIRST TRIANGLE
//		2,3,0,  //SECOND TRIANGLE
//		//BACK SIDE
//		4,5,6,
//		6,7,4,
//		//TOP SIDE
//		8,9,10,
//		10,11,8,
//		//BOTTOM SIDE
//		12,13,14,
//		14,15,12,
//		//RIGHT SIDE
//		16,17,18,
//		18,19,16,
//		//LEFT SIDE
//		20,21,22,
//		22,23,20
//	};
//
//	UINT IndexSize = ARRAYSIZE(index_list);
//
//	ID3D11Buffer* ibuffer = nullptr;
//	{
//
//		D3D11_BUFFER_DESC buff_desc = {};
//		buff_desc.Usage = D3D11_USAGE_IMMUTABLE;
//		buff_desc.ByteWidth = IndexSize * sizeof(unsigned int);
//		buff_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//		buff_desc.CPUAccessFlags = 0;
//		buff_desc.MiscFlags = 0;
//		buff_desc.StructureByteStride = 0;
//
//
//
//		D3D11_SUBRESOURCE_DATA init_data = {};
//		init_data.pSysMem = index_list;
//
//		HRESULT hr = _Device->Get()->CreateBuffer(&buff_desc, &init_data, &ibuffer);
//		if (hr != S_OK)
//		{
//			assert(false);
//		}
//	}
//
//	std::shared_ptr<MH> meshinfo = std::make_shared<MH>();
//	meshinfo->Index = ibuffer;
//	meshinfo->IndexBufferSize = IndexSize;
//	meshinfo->Vertex = vbuffer;
//	meshinfo->BufferSize = sizeof(Vertex);
//
//	AllScene["TEST"]->Meshs["DEFAULT"] = meshinfo;
//
//}



#pragma endregion

ID3D11Buffer* EngineScene::CreateBuffer(UINT _ArraySize, UINT _Size, FBuffer* _List, std::shared_ptr<GraphicDevice> _Device)
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

ID3D11Buffer* EngineScene::CreateIndexBuffer(UINT _ArraySize, UINT _Size, UINT* _List, std::shared_ptr<GraphicDevice> _Device)
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

