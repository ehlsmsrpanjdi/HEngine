#pragma once
#include "Actor.h"
// Ό³Έν :
enum class LightType {
	None,
	Point,
	Directional,
};

class LightActor : public Actor
{
public:
	// constrcuter destructer
	LightActor();
	~LightActor();

	// delete Function
	LightActor(const LightActor& _Other) = delete;
	LightActor(LightActor&& _Other) noexcept = delete;
	LightActor& operator=(const LightActor& _Other) = delete;
	LightActor& operator=(LightActor&& _Other) noexcept = delete;

	XMFLOAT3 Color;
	float Intensity;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	const LightType GetType()
	{
		return Type;
	}

protected:
	LightType Type = LightType::Point;

private:
	friend class Level;
	void SetType(LightType _Type)
	{
		Type = _Type;
	}
};

