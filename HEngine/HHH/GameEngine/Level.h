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
#include "HDirect/ConstantBufferStruct.h"

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
	void BackGroundRender(float _DeltaTime);
	virtual void StartLevel();
	virtual void EndLevel();

	void CameraMatrixUpdate(float _DeltaTime);
	void SetMainCamera(std::string_view _Name);
	void SetMainBackGround(std::string_view _Name);

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
		Camera->BeginPlay();
		AllCamera[str] = Camera;
		if (MainCamera == nullptr) {
			MainCamera = Camera.get();
		}
		return Camera;
	}

	template <typename T>
	std::shared_ptr<T> CreateBackGround(std::string_view _Name) {
		std::string str = HString::Upper(_Name.data());
		if (AllBackGround.contains(str) == true) {
			assert(false);
			return nullptr;
		}
		std::shared_ptr<T> BackGround = std::make_shared<T>();
		BackGround->SetWorld(this);
		BackGround->BeginPlay();
		AllBackGround[str] = BackGround;
		if (MainBackGround == nullptr) {
			MainBackGround = BackGround.get();
		}
		return BackGround;
	}

	template <typename T>
	std::shared_ptr<T> SpawnActor() {
		std::shared_ptr<T> AActor = std::make_shared<T>();
		AActor->SetWorld(this);
		AActor->BeginPlay();
		AllActor.push_back(AActor);
		return AActor;
	}


	std::shared_ptr<class LightActor> CreateLight(enum class LightType _LightType);

	void CollisionCheck();

	class Actor* MainCamera = nullptr;
	class Actor* MainBackGround = nullptr;


	WVP WVPBuffer;
	DirectX::XMMATRIX  WorldMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX ViewMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX PerseMatrix = DirectX::XMMatrixIdentity();


protected:
	friend class Actor;
	friend class Collision;

	std::unordered_map<int, std::list<std::shared_ptr<class Collision>>> Collisions;
	std::list<std::shared_ptr<class Actor>> AllActor;
	std::unordered_map<std::string, std::shared_ptr<class Actor>> AllCamera;
	std::unordered_map<std::string, std::shared_ptr<class Actor>> AllBackGround;
private:

};

