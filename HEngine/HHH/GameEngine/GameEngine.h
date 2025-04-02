#pragma once
#include <iostream>
#include <memory>
#include <list>
#include <map>
#include "Windows.h"
#include "DirectXMath.h"


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

	//template <typename T>
	void CreateCamera(std::string _Name);
	void SetMainCamera(std::string _Name);


	void CameraUpdate(float _DeltaTime);
	std::shared_ptr<class Actor> SpawnActor();

protected:

private:
	class Actor* MainCamera = nullptr;
	GameEngine();

	std::map<std::string, std::shared_ptr<class Actor>> AllCamera;
	std::list<std::shared_ptr<class Actor>> AllActor;


	DirectX::XMMATRIX  WorldMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX ViewMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX PerseMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX WVP = DirectX::XMMatrixIdentity();


	bool isPersepectiveChange = true;
	float FovAngleY = DirectX::XM_PI / 2.0f;    // ���� �þ߰� (����)
	float AspectRatio;  // ȭ�� ���� (����/����)
	float NearZ = 1.f;     // �����
	float FarZ = 1000.f;          // �����
};



