#pragma once
#include "EngineHelper/EngineTransform.h"
#include "unordered_map"
#include "iostream"
#include "functional"
#include "list"

enum class CollisionType {
	None = -1,
	Normal = 0,
	Player = 1,

};

// ¼³¸í :
class Collision : std::enable_shared_from_this<Collision>
{
public:
	// constrcuter destructer
	Collision();
	virtual ~Collision();

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

	void CollisionCheck();


	void AddFunction(std::function<void(void)> _Fun);

	void CollisionRender(float _DeltaTime);
	void SetScene(std::string_view _str);
	void SetHlsl(std::string_view _str);
	void SetCollisionType(CollisionType _Type);
	void SetOwner(class Actor* _Actor);
	std::shared_ptr<class FScene> GetScene();

	CollisionType GetType() { return ColType; }

	void SetType(CollisionType _Type);

	void SetWorld(class Level* _Lv) { World = _Lv; }

	std::list<std::shared_ptr<Collision>>& GetCollisionList(CollisionType _Type);

protected:
	std::vector<std::function<void(void)>> CollisionCheckFunc;
private:
	friend class Actor;
	friend class Level;

	EngineTransform CollisionTransform = EngineTransform();
	struct HS* Hlsl = nullptr;
	std::shared_ptr<class FScene> CollisionScene;

	class Actor* Owner = nullptr;

	class Level* World = nullptr;

	//ºô¸°°Í
	class ID3D11SamplerState* Sampler = nullptr;

	CollisionType ColType = CollisionType::None;
};

