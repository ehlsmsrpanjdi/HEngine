#pragma once
#include <iostream>
#include <memory>
#include <list>
#include <map>
#include "Windows.h"
#include "DirectXMath.h"
#include "EngineHelper/HString.h"


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

	static GameEngine* get() {
		static GameEngine Engine;
		return &Engine;
	}

	std::shared_ptr<class Level> ChangeLevel(std::string_view _Name);

	template <typename T>
	std::shared_ptr<T> CreateLevel(std::string_view _Name) {
		std::string str = HString::Upper(_Name.data());

		std::shared_ptr<T> Lv = std::make_shared<T>();
		if (Levels.contains(str) == false) {
			Levels[str] = std::static_pointer_cast<class Level>(Lv);
		}
		else {
			assert(true);
		}
		Lv->BeginPlay();
		return Lv;
	}



protected:

private:
	friend class Actor;
	friend class Level;
	GameEngine();

	std::map<std::string, std::shared_ptr<class Level>> Levels;

	std::shared_ptr<class Level> SelectedLevel = nullptr;


	DirectX::XMMATRIX  WorldMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX ViewMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX PerseMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX WVP = DirectX::XMMatrixIdentity();


	bool isPersepectiveChange = true;
	float FovAngleY = DirectX::XM_PI / 2.0f;    // ���� �þ߰� (����)
	float AspectRatio = 2.0f;  // ȭ�� ���� (����/����)
	float NearZ = 1.f;     // �����
	float FarZ = 1000.f;          // �����


private:
	std::shared_ptr<class EngineKey> KeyManager = nullptr;
};



