#pragma once
#include "EngineHelper/EngineTransform.h"
#include "unordered_map"
#include "iostream"

// ¼³¸í :
class Actor
{
public:
	// constrcuter destructer
	Actor();
	~Actor();

	// delete Function
	Actor(const Actor& _Other) = delete;
	Actor(Actor&& _Other) noexcept = delete;
	Actor& operator=(const Actor& _Other) = delete;
	Actor& operator=(Actor&& _Other) noexcept = delete;

	virtual void BeginPlay();
	virtual void Tick(float _DeltaTime);
	void Render(float _DeltaTime);

	void AddActorLocation(float _x, float _y = 0, float _z = 0);
	void AddActorScale(float _x, float _y = 0, float _z = 0);
	void AddActorRotation(float _x, float _y = 0, float _z = 0);

	void SetActorLocation(float _x, float _y, float _z);
	void SetActorScale(float _x, float _y, float _z);
	void SetActorRotation(float _x, float _y, float _z);

	void Move(float _x, float _y, float _z);
	void Rotate(float _x, float _y, float _z);

	void SetScene(std::string_view _str);
	void SetHlsl(std::string_view _str);
	void SetAnimation(std::string_view _str);


	class Level* GetWorld();
	class Collision* CreateCollision(enum class CollisionType _Type);

	void SetRoot(Actor* _Act);
	Actor* GetRoot();


	const std::shared_ptr<class FScene> GetScene() const;

	const EngineTransform& GetTransform();
	bool IsAnimation = false;
protected:
	friend class Level;
	std::string Name;

	void SetWorld(class Level* _Lv);
	Actor* RootActor = nullptr;

	float EndAnimTime = 0.0f;
	float CurrentAnimTime = 0.0f;

	EngineTransform ActorTransform = EngineTransform();
	struct HS* Hlsl = nullptr;
	std::vector<DirectX::XMMATRIX> outBoneMatrices;
	//ºô¸°°Í
	std::shared_ptr<class FScene> ActorScene;

	class Level* World = nullptr;
};

