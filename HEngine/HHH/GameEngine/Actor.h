#pragma once
#include "EngineHelper/EngineTransform.h"
#include "unordered_map"
#include "iostream"

// ���� :
class Actor
{
public:
	// constrcuter destructer
	Actor();
	virtual ~Actor();

	// delete Function
	Actor(const Actor& _Other) = delete;
	Actor(Actor&& _Other) noexcept = delete;
	Actor& operator=(const Actor& _Other) = delete;
	Actor& operator=(Actor&& _Other) noexcept = delete;

	virtual void BeginPlay();
	virtual void Tick(float _DeltaTime);
	void Render(float _DeltaTime);
	void BackGroundRender(float _DeltaTime);

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
	void OffAnimation();
	void OnAnimation();
	void StopAnim();
	void RunAnim();


	class Level* GetWorld();
	class Collision* CreateCollision(enum class CollisionType _Type);

	void SetRoot(Actor* _Act);
	Actor* GetRoot();


	const std::shared_ptr<class FScene> GetScene() const;

	const EngineTransform& GetTransform();
private:
	bool IsAnimation = false;
protected:
	friend class Level;
	std::string Name;

	void SetWorld(class Level* _Lv);
	Actor* RootActor = nullptr;

	float EndAnimTime = 0.0f;
	float CurrentAnimTime = 0.0f;
	bool AnimStop = false;

	EngineTransform ActorTransform = EngineTransform();
	struct HS* Hlsl = nullptr;
	std::vector<DirectX::XMMATRIX> outBoneMatrices;
	//������
	std::shared_ptr<class FScene> ActorScene;

	class Level* World = nullptr;

	//������
	class ID3D11SamplerState* Sampler = nullptr;

	std::vector<std::vector<struct KeyFrame>>* SeletedFrame = nullptr;
};

