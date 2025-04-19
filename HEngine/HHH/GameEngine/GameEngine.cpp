#include "GameEngine.h"
#include "Actor.h"
#include <cassert>
#include "HDirect/GraphicsEngine.h"
#include "EngineHelper/EngineDebug.h"	
#include "EngineHelper/EngineKey.h"
#include "EngineHelper/EngineNamespace.h"


#include "WoodenBox.h"
#include "MainPlayer.h"

GameEngine::GameEngine()
{
}

GameEngine::~GameEngine()
{
	AllActor.clear();
	AllCamera.clear();
	KeyManager = nullptr;
}

void GameEngine::Init(RECT _rc)
{
	float width = (float)(_rc.right - _rc.left);
	float height = (float)(_rc.bottom - _rc.top);

	AspectRatio = width / height;

	if (isPersepectiveChange == true) {
		isPersepectiveChange = false;
		PerseMatrix = XMMatrixPerspectiveFovLH(FovAngleY, AspectRatio, NearZ, FarZ);
	}

	KeyManager = std::make_shared<EngineKey>();

	KeyManager->Init();

	CreateCamera<Actor>("temp");
	SetMainCamera("temp");

	SpawnActor<MainPlayer>();
	SpawnActor<WoodenBox>();
}

void GameEngine::release()
{
	AllCamera.clear();
	AllActor.clear();
}

void GameEngine::Update(float _DeltaTime)
{
	if (isPersepectiveChange == true) {
		isPersepectiveChange = false;
		PerseMatrix = XMMatrixPerspectiveFovLH(FovAngleY, AspectRatio, NearZ, FarZ);
	}
	CameraUpdate(_DeltaTime);
	EngineTransform trasn;
	if (EngineKey::IsPressed('W')) {
		MainCamera->Move(0.0f, 0.0f, 10.f * _DeltaTime);
	}
	if (EngineKey::IsPressed('S')) {
		MainCamera->Move(0.0f, 0.0f, -10.f * _DeltaTime);
	}
	if (EngineKey::IsPressed('A')) {
		MainCamera->Move(-10.f * _DeltaTime, 0.0f, 0.0f);
	}
	if (EngineKey::IsPressed('D')) {
		MainCamera->Move(10.f * _DeltaTime, 0.0f, 0.0f);
	}

	if (EngineKey::IsPressed(VK_RBUTTON)) {
		MainCamera->Rotate(EngineKey::MouseX * _DeltaTime, EngineKey::MouseY  * _DeltaTime, 0.f);
	}

	for (std::shared_ptr<Actor> Act : AllActor) {
		Act->Tick(_DeltaTime);
	}

	Render(_DeltaTime);
}

void GameEngine::Render(float _DeltaTime)
{
	GraphicsEngine::get()->Clear(1.0f, 0.5, 0.5, 1.0f);
	EngineTransform trans;
	for (std::shared_ptr<Actor> Act : AllActor) {
		WorldMatrix = Act->GetTransform().GetWorldMatrix();
		WVP = WorldMatrix * ViewMatrix * PerseMatrix;
		GraphicsEngine::get()->UpdateConstantBuffer(WVP, Cbuffer::WVP);
		Act->Render(_DeltaTime);
	}
	GraphicsEngine::get()->Present(true);
}

void GameEngine::SetMainCamera(std::string _Name)
{
	std::string str = HString::Upper(_Name);
	if (AllCamera.contains(str) == true) {
		MainCamera = AllCamera[str].get();
	}
	return;
}

void GameEngine::CameraUpdate(float _DeltaTime)
{
	for (std::pair<const std::string, std::shared_ptr<Actor>>& pa : AllCamera) {
		pa.second->Tick(_DeltaTime);
	}

	if (MainCamera == nullptr) {
		//EngineDebug::Error("카메라 없는데 업데이트중");
		assert(false);
		return;
	}
	else {
		ViewMatrix = MainCamera->GetTransform().GetInverseMatrix();
	}

}


