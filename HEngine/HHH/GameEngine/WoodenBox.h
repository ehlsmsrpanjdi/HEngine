#pragma once
#include "Actor.h"
// Ό³Έν :
class WoodenBox : public Actor
{
public:
	// constrcuter destructer
	WoodenBox();
	~WoodenBox();

	// delete Function
	WoodenBox(const WoodenBox& _Other) = delete;
	WoodenBox(WoodenBox&& _Other) noexcept = delete;
	WoodenBox& operator=(const WoodenBox& _Other) = delete;
	WoodenBox& operator=(WoodenBox&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _deltatime) override;
protected:
private:

};

