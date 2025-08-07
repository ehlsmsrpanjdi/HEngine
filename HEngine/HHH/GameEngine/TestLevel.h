#pragma once
#include "Level.h"
// 설명 :
class TestLevel : public Level
{
public:
	// constrcuter destructer
	TestLevel();
	~TestLevel();

	// delete Function
	TestLevel(const TestLevel& _Other) = delete;
	TestLevel(TestLevel&& _Other) noexcept = delete;
	TestLevel& operator=(const TestLevel& _Other) = delete;
	TestLevel& operator=(TestLevel&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;


protected:

private:
	std::shared_ptr<class MainPlayer> mainPlayer = nullptr;
};

