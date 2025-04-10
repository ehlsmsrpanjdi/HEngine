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
	
protected:


private:
	friend class EngineScene;
	friend class EngineSkeleton;

	std::vector<struct FBuffer> vertices; // vertex buffer
	std::vector<UINT> indices;

	std::string TextureName;

	int GetSize();

	std::shared_ptr<class EngineSkeleton> Skeleton = nullptr;
	FbxAMatrix tempmatrix = FbxAMatrix();
};

