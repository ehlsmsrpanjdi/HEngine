#pragma once

// Ό³Έν :
class EngineAnimation
{
public:
	// constrcuter destructer
	EngineAnimation();
	~EngineAnimation();

	// delete Function
	EngineAnimation(const EngineAnimation& _Other) = delete;
	EngineAnimation(EngineAnimation&& _Other) noexcept = delete;
	EngineAnimation& operator=(const EngineAnimation& _Other) = delete;
	EngineAnimation& operator=(EngineAnimation&& _Other) noexcept = delete;

protected:

private:

};

