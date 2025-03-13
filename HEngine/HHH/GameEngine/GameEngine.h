#pragma once
#include <iostream>
#include <memory>
#include <list>
#include <map>
// ���� :
class GameEngine
{
public:
	// constrcuter destructer
	~GameEngine();

	// delete Function
	GameEngine(const GameEngine& _Other) = delete;
	GameEngine(GameEngine&& _Other) noexcept = delete;
	GameEngine& operator=(const GameEngine& _Other) = delete;
	GameEngine& operator=(GameEngine&& _Other) noexcept = delete;

	void Init();
	void release();

	void Update(float _DeltaTime);

	static GameEngine* get() {
		static GameEngine Engine;
		return &Engine;
	}

	void test() {
		std::cout << "asasdfdf";
	}

	void CreateCamera(std::string _Name);
	void SetMainCamera(std::string _Name);

	class std::shared_ptr<class Actor> SpawnActor();

	std::map<std::string, class Actor*> AllCamera;
	std::list<std::shared_ptr<class Actor>> AllActor;

protected:

private:
	class Actor* MainCamera = nullptr;
	GameEngine();
};



