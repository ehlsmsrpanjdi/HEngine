#pragma once
#include "Actor.h"
// Ό³Έν :
class MainPlayer : public Actor
{
public:
	// constrcuter destructer
	MainPlayer();
	~MainPlayer();

	// delete Function
	MainPlayer(const MainPlayer& _Other) = delete;
	MainPlayer(MainPlayer&& _Other) noexcept = delete;
	MainPlayer& operator=(const MainPlayer& _Other) = delete;
	MainPlayer& operator=(MainPlayer&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _deltatime) override;
protected:

private:

};

