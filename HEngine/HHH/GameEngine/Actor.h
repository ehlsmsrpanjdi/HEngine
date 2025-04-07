#pragma once
#include "EngineHelper/EngineTransform.h"
#include "unordered_map"
#include "iostream"

// Ό³Έν :
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

	void BeginPlay();
	void Tick(float _DeltaTime);
	void Render();

	void AddActorLocation(float _x, float _y = 0, float _z = 0);
	void AddActorScale(float _x, float _y = 0, float _z = 0);
	void AddActorRotation(float _x, float _y = 0, float _z = 0);

	void SetActorLocation(float _x, float _y, float _z);
	void SetActorScale(float _x, float _y, float _z);
	void SetActorRotation(float _x, float _y, float _z);

	void Move(float _x, float _y, float _z);
	void Rotate(float _x, float _y, float _z);

	void SetMesh(std::string_view _str);
	void SetHlsl(std::string_view _str);

	EngineTransform GetTransform();
protected:

private:
	EngineTransform ActorTransform = EngineTransform();
	struct HS* Hlsl = nullptr;
	std::unordered_map<std::string, std::shared_ptr<struct MH>>* Meshs = nullptr;
};

