#pragma once
#include "Actor.h"
// Ό³Έν :
class TestActor : public Actor
{
public:
	// constrcuter destructer
	TestActor();
	~TestActor();

	// delete Function
	TestActor(const TestActor& _Other) = delete;
	TestActor(TestActor&& _Other) noexcept = delete;
	TestActor& operator=(const TestActor& _Other) = delete;
	TestActor& operator=(TestActor&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _deltatime) override;
protected:

private:

};

