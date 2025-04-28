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

	void AddLocation(float _x, float _y = 0, float _z = 0);
	void AddScale(float _x, float _y = 0, float _z = 0);
	void AddRotation(float _x, float _y = 0, float _z = 0);

	void SetLocation(float _x, float _y, float _z);
	void SetScale(float _x, float _y, float _z);
	void SetRotation(float _x, float _y, float _z);

	void Move(float _x, float _y, float _z);
	void Rotate(float _x, float _y, float _z);

	const EngineTransform& GetTransform();

	void CollisionRender(float _DeltaTime);
	void SetScene(std::string_view _str);
	void SetHlsl(std::string_view _str);
	void SetCollisionType(CollisionType _Type);
	void SetOwner(class Actor* _Actor);
	std::shared_ptr<class FScene> GetScene();

protected:

private:
	friend class Actor;

	EngineTransform CollisionTransform = EngineTransform();
	struct HS* Hlsl = nullptr;
	std::shared_ptr<class FScene> CollisionScene;

	class Actor* Owner = nullptr;

	CollisionType ColType = CollisionType::None;
};

