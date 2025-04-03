#include "GameEngine.h"
#include "Actor.h"
#include "assert.h"
#include "HDirect/GraphicsEngine.h"
#include "EngineHelper/EngineDebug.h"	
#include "EngineHelper/EngineKey.h"

GameEngine::GameEngine()
{
}

GameEngine::~GameEngine()
{
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

	Actor* act = SpawnActor<Actor>().get();
	act->SetMesh("cube");
	act->SetMaterial("Basic");
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
		MainCamera->AddActorLocation(0.0f, 0.0f, 1.f * _DeltaTime);
	}
	if (EngineKey::IsPressed('S')) {
		MainCamera->AddActorLocation(0.0f, 0.0f, -1.f * _DeltaTime);
	}
	if (EngineKey::IsPressed('A')) {
		MainCamera->AddActorLocation(-1.f * _DeltaTime, 0.0f, 0.0f);
	}
	if (EngineKey::IsPressed('D')) {
		MainCamera->AddActorLocation(1.f * _DeltaTime, 0.0f, 0.0f);
	}

	if (EngineKey::IsPressed(VK_RBUTTON)) {
		MainCamera->AddActorRotation(EngineKey::MouseX * _DeltaTime, EngineKey::MouseY  * _DeltaTime, 0.f);
	}

	//for (std::shared_ptr<Actor> Act : AllActor) {
	//	Act->Tick(_DeltaTime);
	//}

	Render();
}

void GameEngine::Render()
{
	GraphicsEngine::get()->Clear(1.0f, 0.5, 0.5, 1.0f);
	EngineTransform trans;
	for (std::shared_ptr<Actor> Act : AllActor) {
		WorldMatrix = Act->GetTransform().GetWorldMatrix();
		WVP = WorldMatrix * ViewMatrix * PerseMatrix;
		GraphicsEngine::get()->UpdateConstantBuffer(WVP, "WVPMatrix");
		Act->Render();
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
		EngineDebug::Error("카메라 없는데 업데이트중");
		exit(EXIT_FAILURE);
		return;
	}
	else {
		ViewMatrix = MainCamera->GetTransform().GetInverseMatrix();
	}

}


