#pragma once

// Ό³Έν :
class FScene
{
public:
	// constrcuter destructer
	FScene();
	~FScene();

	// delete Function
	FScene(const FScene& _Other) = delete;
	FScene(FScene&& _Other) noexcept = delete;
	FScene& operator=(const FScene& _Other) = delete;
	FScene& operator=(FScene&& _Other) noexcept = delete;

protected:

private:

};

