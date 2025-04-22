#pragma once
#include "EngineHelper/EngineTransform.h"
#include "unordered_map"
#include "iostream"

enum class CollisionType {
	None = -1,
	Normal = 0,

};

// Ό³Έν :
class Collision
{
public:
	// constrcuter destructer
	Collision();
	~Collision();

	// delete Function
	Collision(const Collision& _Other) = delete;
	Collision(Collision&& _Other) noexcept = delete;
	Collision& operator=(const Collision& _Other) = delete;
	Collision& operator=(Collision&& _Other) noexcept = delete;


	void SetScene(std::string_view _str);
	void SetHlsl(std::string_view _str);
	void SetCollisionType(CollisionType _Type);
	void SetOwner(class Actor* _Actor);

protected:

private:
	friend class Actor;

	EngineTransform ActorTransform = EngineTransform();
	struct HS* Hlsl = nullptr;
	std::vector<DirectX::XMMATRIX> outBoneMatrices;
	std::shared_ptr<class FScene> CollisionScene;

	class Actor* Owner = nullptr;

	CollisionType ColType = CollisionType::None;
};

