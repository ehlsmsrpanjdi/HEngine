#pragma once
#include "Actor.h"

class BoxActor : public Actor
{
public:
	// constrcuter destructer
	BoxActor();
	~BoxActor();

	// delete Function
	BoxActor(const BoxActor& _Other) = delete;
	BoxActor(BoxActor&& _Other) noexcept = delete;
	BoxActor& operator=(const BoxActor& _Other) = delete;
	BoxActor& operator=(BoxActor&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _deltatime) override;

protected:
	Collision* boxcollision = nullptr;
};

