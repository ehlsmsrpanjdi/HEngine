#pragma once
#include "Actor.h"
// Ό³Έν :
class TestTestActor : public Actor
{
public:
	// constrcuter destructer
	TestTestActor();
	~TestTestActor();

	// delete Function
	TestTestActor(const TestTestActor& _Other) = delete;
	TestTestActor(TestTestActor&& _Other) noexcept = delete;
	TestTestActor& operator=(const TestTestActor& _Other) = delete;
	TestTestActor& operator=(TestTestActor&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _deltatime) override;
protected:

private:

};

