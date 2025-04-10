#pragma once

// Ό³Έν :
class EngineScene
{
public:
	// constrcuter destructer
	EngineScene();
	~EngineScene();

	// delete Function
	EngineScene(const EngineScene& _Other) = delete;
	EngineScene(EngineScene&& _Other) noexcept = delete;
	EngineScene& operator=(const EngineScene& _Other) = delete;
	EngineScene& operator=(EngineScene&& _Other) noexcept = delete;

protected:

private:

};

