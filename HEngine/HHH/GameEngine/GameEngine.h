#pragma once
#include <iostream>
#include <memory>
#include <list>
#include <map>
#include "Windows.h"
#include "DirectXMath.h"


// 설명 :
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

	void Init(RECT _rc);
	void release();

	void Update(float _DeltaTime);
	void Render();

	static GameEngine* get() {
		static GameEngine Engine;
		return &Engine;
	}

	void test() {
		std::cout << "asasdfdf";
	}

	void CreateCamera(std::string _Name);
	void SetMainCamera(std::string _Name);


	void CameraUpdate();
	std::shared_ptr<class Actor> SpawnActor();

protected:

private:
	class Actor* MainCamera = nullptr;
	GameEngine();

	std::map<std::string, class Actor*> AllCamera;
	std::list<std::shared_ptr<class Actor>> AllActor;


	DirectX::XMMATRIX  WorldMatrix;
	DirectX::XMMATRIX ViewMatrix;
	DirectX::XMMATRIX PerseMatrix;
	DirectX::XMMATRIX WVP;
	//EngineTransform;


	bool isPersepectiveChange = true;
	float FovAngleY = DirectX::XM_PI / 2.0f;    // 수직 시야각 (라디안)
	float AspectRatio;  // 화면 비율 (가로/세로)
	float NearZ = 1.f;     // 근평면
	float FarZ = 1000.f;          // 원평면
};



