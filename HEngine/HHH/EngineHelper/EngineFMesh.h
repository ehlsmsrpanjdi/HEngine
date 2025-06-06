#pragma once
#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "unordered_map"
#include <Windows.h>

#include "fbxsdk.h"
#include <unordered_set>
// ���� :
class EngineFMesh
{
public:
	// constrcuter destructer
	EngineFMesh();
	~EngineFMesh();

	// delete Function
	EngineFMesh(const EngineFMesh& _Other) = delete;
	EngineFMesh(EngineFMesh&& _Other) noexcept = delete;
	EngineFMesh& operator=(const EngineFMesh& _Other) = delete;
	EngineFMesh& operator=(EngineFMesh&& _Other) noexcept = delete;


	void init(FbxNode* pNode);
	void BoneValueDelete();

	
	UINT GetSize();
	UINT GetArraySize();

	UINT GetIndexSize();
	UINT GetIndexArraySize();

	const std::vector<struct FBuffer>& GetVertex();
	const std::vector<UINT>& GetIndices();

	std::string_view GetTextureName()
	{
		return TextureName;
	}

	FbxAMatrix MeshMatrix = FbxAMatrix();
protected:


private:
	friend class EngineFScene;
	friend class EngineFSkeleton;

	std::vector<struct FBuffer> vertices; // vertex buffer
	std::vector<UINT> indices;

	std::string TextureName;

};

