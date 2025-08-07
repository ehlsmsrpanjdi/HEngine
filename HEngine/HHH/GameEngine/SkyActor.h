#pragma once
#include "Actor.h"
// 설명 :
class SkyActor : public Actor
{
public:
	// constrcuter destructer
	SkyActor();
	~SkyActor();

	// delete Function
	SkyActor(const SkyActor& _Other) = delete;
	SkyActor(SkyActor&& _Other) noexcept = delete;
	SkyActor& operator=(const SkyActor& _Other) = delete;
	SkyActor& operator=(SkyActor&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _deltatime) override;
protected:

private:

};

