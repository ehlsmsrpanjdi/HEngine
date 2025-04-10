#pragma once
#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "unordered_map"
#include <Windows.h>

#include "fbxsdk.h"
#include <unordered_set>
// Ό³Έν :
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
	
	int GetSize();
	int GetArraySize();

	int GetIndexSize();
	int GetIndexArraySize();

	const std::vector<struct FBuffer>& GetVertex();
	const std::vector<UINT>& GetIndices();

	std::string_view GetTextureName()
	{
		return TextureName;
	}

protected:


private:
	friend class EngineFScene;
	friend class EngineFSkeleton;

	std::vector<struct FBuffer> vertices; // vertex buffer
	std::vector<UINT> indices;

	std::string TextureName;


	std::shared_ptr<class EngineFSkeleton> Skeleton = nullptr;
	FbxAMatrix tempmatrix = FbxAMatrix();
};

