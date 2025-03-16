#pragma once
#include "EngineHelper/EngineTransform.h"
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

	EngineTransform GetTransform();
protected:

private:
	EngineTransform ActorTransform = EngineTransform();
};

