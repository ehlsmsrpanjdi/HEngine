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
	AllMeshMap.clear();
}

void EngineMesh::CreateMesh(std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<FMesh>>>& _AllMesh, std::shared_ptr<class GraphicDevice> _Device)
{
	for (std::pair<const std::string, std::unordered_map<std::string, std::shared_ptr<FMesh>>>& mesh : _AllMesh) {
		for (std::pair<const std::string, std::shared_ptr<FMesh>> meshinfo : mesh.second) {
			UINT Size = sizeof(meshinfo.second->vertices[0]);
			UINT arraysize = static_cast<UINT>(meshinfo.second->vertices.size());

			std::shared_ptr<MH> Mesh = std::make_shared<MH>();
			Mesh->BufferSize = Size;
			Mesh->Vertex = CreateBuffer(arraysize, Size, (UINT*)meshinfo.second->vertices.data(), _Device);

			Size = sizeof(meshinfo.second->indices[0]);
			arraysize = static_cast<UINT>(meshinfo.second->indices.size());
			Mesh->IndexBufferSize = arraysize;
			Mesh->Index = CreateIndexBuffer(arraysize, Size, meshinfo.second->indices.data(), _Device);

			std::unordered_map<std::string, std::shared_ptr<MH>>& MeshMap = AllMeshMap[mesh.first];

			if (MeshMap.contains(meshinfo.first) == false) {
				AllMeshMap[mesh.first][meshinfo.first] = Mesh;
			}
			else {
				assert(false);
			}
		}
	}

	Test(_Device);
	Test2(_Device);
}

std::unordered_map<std::string, std::shared_ptr<MH>>& EngineMesh::GetMesh(std::string_view _str)
{
	std::string str = HString::Upper(_str.data());
	if (AllMeshMap.contains(str) == false) {
		assert(false);
	}
	return AllMeshMap[str];
}

#pragma region "테스트용"
struct Vertex {
	DirectX::XMFLOAT3 position; // 위치값 (x, y, z)
	DirectX::XMFLOAT2 uv;      // 텍스처 좌표 (u, v)
};

void EngineMesh::Test(std::shared_ptr<GraphicDevice> _Device)
{
	DirectX::XMFLOAT3 position_list[] =
	{
		{ DirectX::XMFLOAT3(-0.5f,-0.5f,-0.5f)},
		{ DirectX::XMFLOAT3(-0.5f,0.5f,-0.5f) },
		{ DirectX::XMFLOAT3(0.5f,0.5f,-0.5f) },
		{ DirectX::XMFLOAT3(0.5f,-0.5f,-0.5f)},

		//BACK FACE
		{ DirectX::XMFLOAT3(0.5f,-0.5f,0.5f) },
		{ DirectX::XMFLOAT3(0.5f,0.5f,0.5f) },
		{ DirectX::XMFLOAT3(-0.5f,0.5f,0.5f)},
		{ DirectX::XMFLOAT3(-0.5f,-0.5f,0.5f) }
	};

	DirectX::XMFLOAT2 texcoord_list[] =
	{
		{ DirectX::XMFLOAT2(0.0f,0.0f) },
		{ DirectX::XMFLOAT2(0.0f,1.0f) },
		{ DirectX::XMFLOAT2(1.0f,0.0f) },
		{ DirectX::XMFLOAT2(1.0f,1.0f) }
	};



	Vertex vertex_list[] =
	{
		//X - Y - Z
		//FRONT FACE
		{ position_list[0],texcoord_list[1] },
		{ position_list[1],texcoord_list[0] },
		{ position_list[2],texcoord_list[2] },
		{ position_list[3],texcoord_list[3] },


		{ position_list[4],texcoord_list[1] },
		{ position_list[5],texcoord_list[0] },
		{ position_list[6],texcoord_list[2] },
		{ position_list[7],texcoord_list[3] },


		{ position_list[1],texcoord_list[1] },
		{ position_list[6],texcoord_list[0] },
		{ position_list[5],texcoord_list[2] },
		{ position_list[2],texcoord_list[3] },

		{ position_list[7],texcoord_list[1] },
		{ position_list[0],texcoord_list[0] },
		{ position_list[3],texcoord_list[2] },
		{ position_list[4],texcoord_list[3] },

		{ position_list[3],texcoord_list[1] },
		{ position_list[2],texcoord_list[0] },
		{ position_list[5],texcoord_list[2] },
		{ position_list[4],texcoord_list[3] },

		{ position_list[7],texcoord_list[1] },
		{ position_list[6],texcoord_list[0] },
		{ position_list[1],texcoord_list[2] },
		{ position_list[0],texcoord_list[3] }
	};

	


	UINT size_list = ARRAYSIZE(vertex_list);




		ID3D11Buffer* vbuffer = nullptr;
	{
		D3D11_BUFFER_DESC buff_desc = {};
		buff_desc.Usage = D3D11_USAGE_DEFAULT;
		buff_desc.ByteWidth = size_list * sizeof(Vertex);
		buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buff_desc.CPUAccessFlags = 0;
		buff_desc.MiscFlags = 0;


		D3D11_SUBRESOURCE_DATA init_data = {};
		init_data.pSysMem = vertex_list;

		HRESULT hr = _Device->Get()->CreateBuffer(&buff_desc, &init_data, &vbuffer);

		if (hr != S_OK)
		{
			assert(false);
		}
	}


	unsigned int index_list[] =
	{
		//FRONT SIDE
		0,1,2,  //FIRST TRIANGLE
		2,3,0,  //SECOND TRIANGLE
		//BACK SIDE
		4,5,6,
		6,7,4,
		//TOP SIDE
		8,9,10,
		10,11,8,
		//BOTTOM SIDE
		12,13,14,
		14,15,12,
		//RIGHT SIDE
		16,17,18,
		18,19,16,
		//LEFT SIDE
		20,21,22,
		22,23,20
	};

	UINT IndexSize = ARRAYSIZE(index_list);

	ID3D11Buffer* ibuffer = nullptr;
	{

		D3D11_BUFFER_DESC buff_desc = {};
		buff_desc.Usage = D3D11_USAGE_IMMUTABLE;
		buff_desc.ByteWidth = IndexSize * sizeof(unsigned int);
		buff_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		buff_desc.CPUAccessFlags = 0;
		buff_desc.MiscFlags = 0;
		buff_desc.StructureByteStride = 0;



		D3D11_SUBRESOURCE_DATA init_data = {};
		init_data.pSysMem = index_list;

		HRESULT hr = _Device->Get()->CreateBuffer(&buff_desc, &init_data, &ibuffer);
		if (hr != S_OK)
		{
			assert(false);
		}
	}

	std::shared_ptr<MH> meshinfo = std::make_shared<MH>();
	meshinfo->Index = ibuffer;
	meshinfo->IndexBufferSize = IndexSize;
	meshinfo->Vertex = vbuffer;
	meshinfo->BufferSize = sizeof(Vertex);

	AllMeshMap["TEST"]["DEFAULT"] = meshinfo;

}

struct Vertex2 {
	DirectX::XMFLOAT3 position; // 위치값 (x, y, z)
};


void EngineMesh::Test2(std::shared_ptr<GraphicDevice> _Device)
{
	DirectX::XMFLOAT3 position_list[] =
	{
		{ DirectX::XMFLOAT3(-0.5f,-0.5f,-0.5f)},
		{ DirectX::XMFLOAT3(-0.5f,0.5f,-0.5f) },
		{ DirectX::XMFLOAT3(0.5f,0.5f,-0.5f) },
		{ DirectX::XMFLOAT3(0.5f,-0.5f,-0.5f)},

		//BACK FACE
		{ DirectX::XMFLOAT3(0.5f,-0.5f,0.5f) },
		{ DirectX::XMFLOAT3(0.5f,0.5f,0.5f) },
		{ DirectX::XMFLOAT3(-0.5f,0.5f,0.5f)},
		{ DirectX::XMFLOAT3(-0.5f,-0.5f,0.5f) }
	};

	Vertex2 vertex_list[] =
	{
		//X - Y - Z
		//FRONT FACE
		{ position_list[0]},
		{ position_list[1]},
		{ position_list[2]},
		{ position_list[3]},


		{ position_list[4]},
		{ position_list[5]},
		{ position_list[6]},
		{ position_list[7]},


		{ position_list[1]},
		{ position_list[6]},
		{ position_list[5]},
		{ position_list[2]},

		{ position_list[7]},
		{ position_list[0]},
		{ position_list[3]},
		{ position_list[4]},

		{ position_list[3]},
		{ position_list[2]},
		{ position_list[5]},
		{ position_list[4]},

		{ position_list[7]},
		{ position_list[6]},
		{ position_list[1]},
		{ position_list[0] }
	};




	UINT size_list = ARRAYSIZE(vertex_list);




	ID3D11Buffer* vbuffer = nullptr;
	{
		D3D11_BUFFER_DESC buff_desc = {};
		buff_desc.Usage = D3D11_USAGE_DEFAULT;
		buff_desc.ByteWidth = size_list * sizeof(Vertex2);
		buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buff_desc.CPUAccessFlags = 0;
		buff_desc.MiscFlags = 0;


		D3D11_SUBRESOURCE_DATA init_data = {};
		init_data.pSysMem = vertex_list;

		HRESULT hr = _Device->Get()->CreateBuffer(&buff_desc, &init_data, &vbuffer);

		if (hr != S_OK)
		{
			assert(false);
		}
	}


	unsigned int index_list[] =
	{
		//FRONT SIDE
		0,1,2,  //FIRST TRIANGLE
		2,3,0,  //SECOND TRIANGLE
		//BACK SIDE
		4,5,6,
		6,7,4,
		//TOP SIDE
		8,9,10,
		10,11,8,
		//BOTTOM SIDE
		12,13,14,
		14,15,12,
		//RIGHT SIDE
		16,17,18,
		18,19,16,
		//LEFT SIDE
		20,21,22,
		22,23,20
	};

	UINT IndexSize = ARRAYSIZE(index_list);

	ID3D11Buffer* ibuffer = nullptr;
	{

		D3D11_BUFFER_DESC buff_desc = {};
		buff_desc.Usage = D3D11_USAGE_IMMUTABLE;
		buff_desc.ByteWidth = IndexSize * sizeof(unsigned int);
		buff_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		buff_desc.CPUAccessFlags = 0;
		buff_desc.MiscFlags = 0;
		buff_desc.StructureByteStride = 0;



		D3D11_SUBRESOURCE_DATA init_data = {};
		init_data.pSysMem = index_list;

		HRESULT hr = _Device->Get()->CreateBuffer(&buff_desc, &init_data, &ibuffer);
		if (hr != S_OK)
		{
			assert(false);
		}
	}

	std::shared_ptr<MH> meshinfo = std::make_shared<MH>();
	meshinfo->Index = ibuffer;
	meshinfo->IndexBufferSize = IndexSize;
	meshinfo->Vertex = vbuffer;
	meshinfo->BufferSize = sizeof(Vertex2);

	AllMeshMap["TEST2"]["DEFAULT"] = meshinfo;
}
#pragma endregion

ID3D11Buffer* EngineMesh::CreateBuffer(UINT _ArraySize, UINT _Size, UINT* _List, std::shared_ptr<GraphicDevice> _Device)
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

ID3D11Buffer* EngineMesh::CreateIndexBuffer(UINT _ArraySize, UINT _Size, UINT* _List, std::shared_ptr<GraphicDevice> _Device)
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

