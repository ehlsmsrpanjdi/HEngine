#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <vector>
#include <map>
#include "iostream"
#include <memory>
#include <unordered_map>


class FScene {
public:
	std::unordered_map<std::string, std::shared_ptr<struct MH>> Meshs;
	std::shared_ptr<class EngineAnimatinSkeleton> AnimSkeleton = nullptr;
	~FScene() {
		Meshs.clear();
	}
};

// Ό³Έν :
class EngineScene
{

public:
	// constrcuter destructer
	~EngineScene();

	static EngineScene& Get() {
		static EngineScene Mesh;
		return Mesh;
	}

	// delete Function
	EngineScene(const EngineScene& _Other) = delete;
	EngineScene(EngineScene&& _Other) noexcept = delete;
	EngineScene& operator=(const EngineScene& _Other) = delete;
	EngineScene& operator=(EngineScene&& _Other) noexcept = delete;

	void CreateScene(std::vector<std::shared_ptr<class EngineFScene>>& _Scenes, std::shared_ptr<class GraphicDevice> _Device);
	std::shared_ptr<FScene> GetScene(std::string_view _str);



	//void Test(std::shared_ptr<GraphicDevice> _Device);

protected:
	ID3D11Buffer* CreateBuffer(UINT _ArraySize, UINT _Size, struct FBuffer* _List, std::shared_ptr<class GraphicDevice> _Device);
	ID3D11Buffer* CreateIndexBuffer(UINT _ArraySize, UINT _Size, UINT* _List, std::shared_ptr<class GraphicDevice> _Device);
private:
	EngineScene();

	std::unordered_map<std::string, std::shared_ptr<FScene>> AllScene;
};

