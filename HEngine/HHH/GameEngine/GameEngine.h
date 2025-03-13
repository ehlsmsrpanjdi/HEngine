#pragma once
#include <iostream>
#include <memory>
#include <list>
#include <map>
// Ό³Έν :
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


	static GameEngine* get() {
		static GameEngine Engine;
		return &Engine;
	}

	void test() {
		std::cout << "asasdfdf";
	}

	void CreateCamera(std::string _Name);
	void SetCamera(std::string _Name);

	class std::shared_ptr<class Actor> SpawnActor();

	std::map<std::string, class Actor*> AllCamera;
	std::list<std::shared_ptr<class Actor>> AllActor;

protected:

private:
	class Actor* MainCamera = nullptr;
	GameEngine();
};



