#pragma once
#include "Actor.h"
// 설명 :
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

	void SetPlayerAnimation(const std::string& _string);
	void CalculateFirction();

	float ForwardSpeed = 0.f;
	float LeftSpeed = 0.f;
private:
	Collision* boxcollision = nullptr;

	std::string CurrentAnimationName;

	const std::string IdleAnimationName = "Idle";
	const std::string RunAnimationName = "Run";
	const std::string JumpAnimationName = "Jump";
};

