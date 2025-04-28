#pragma once
#include <iostream>
#include <memory>
#include <list>
#include <map>
#include "Windows.h"
#include "DirectXMath.h"
#include "EngineHelper/HString.h"
#include "vector"
#include "unordered_map"


// Ό³Έν :
class Level
{
public:
	// constrcuter destructer
	Level();
	~Level();

	// delete Function
	Level(const Level& _Other) = delete;
	Level(Level&& _Other) noexcept = delete;
	Level& operator=(const Level& _Other) = delete;
	Level& operator=(Level&& _Other) noexcept = delete;


	virtual void BeginPlay();
	virtual void EndPlay();
	virtual void Tick(float _DeltaTime);
	void Render(float _DeltaTime);
	void CollisionRender(float _DeltaTime);
	void SetMainCamera(std::string_view _Name);
	virtual void StartLevel();
	virtual void EndLevel();

	void CameraMatrixUpdate(float _DeltaTime);

	template <typename T>
	std::shared_ptr<T> CreateCamera(std::string_view _Name) {
		std::string str = HString::Upper(_Name.data());
		if (AllCamera.contains(str) == true) {
			assert(false);
			return nullptr;
		}
		std::shared_ptr<T> Camera = std::make_shared<T>();
		Camera->SetWorld(this);
		AllActor.push_back(Camera);
		AllCamera[str] = Camera;
		return Camera;
	}

	template <typename T>
	std::shared_ptr<T> SpawnActor() {
		std::shared_ptr<T> AActor = std::make_shared<T>();
		AActor->SetWorld(this);
		AActor->BeginPlay();
		AllActor.push_back(AActor);
		return AActor;
	}



	class Actor* MainCamera = nullptr;

	DirectX::XMMATRIX  WorldMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX ViewMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX PerseMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX WVP = DirectX::XMMatrixIdentity();


protected:
	friend class Actor;

	std::unordered_map<int, std::vector<std::shared_ptr<class Collision>>> Collisions;
	std::list<std::shared_ptr<class Actor>> AllActor;
	std::unordered_map<std::string, std::shared_ptr<class Actor>> AllCamera;
private:

};

