#include "GameEngine.h"
#include "Actor.h"
#include "assert.h"
#include "EngineHelper/HString.h"
#include "HDirect/GraphicsEngine.h"
#include "EngineHelper/EngineDebug.h"	

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



	CreateCamera("temp");
	SetMainCamera("temp");

	//MainCamera->AddActorLocation(0.f, 0.f, -2.f);
	//MainCamera->AddActorRotation(0.f, 0.f, 45.f);

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
	CameraUpdate();
	EngineTransform trasn;
	MainCamera->AddActorLocation(0.01f, 0.0f, 0.f);
	//for (std::shared_ptr<Actor> Act : AllActor) {
	//	Act->Tick(_DeltaTime);
	//}

	Render();
}

void GameEngine::Render()
{
	EngineTransform trans;
	//trans.SetScale(2.0f, 2.0f, 1.0f);
	//WorldMatrix = DirectX::XMMatrixIdentity();
	WVP = trans.GetWorldMatrix() * ViewMatrix * PerseMatrix;
	GraphicsEngine::get()->UpdateConstantBuffer(WVP, "WVPMatrix");
	//for (std::shared_ptr<Actor> Act : AllActor) {
	//	WorldMatrix = Act->GetTransform().GetWorldMatrix();
	//	WVP = WorldMatrix* ViewMatrix* PerseMatrix;
	//	GraphicsEngine::get()->UpdateConstantBuffer(WVP, "WVPMatrix");
	//}
}

void GameEngine::CreateCamera(std::string _Name)
{
	std::string str = HString::Upper(_Name);
	if (AllCamera.contains(str) == true) {
		return;
	}
	std::shared_ptr<Actor> Camera = SpawnActor();
	AllCamera[str] = Camera.get();
}

void GameEngine::SetMainCamera(std::string _Name)
{
	std::string str = HString::Upper(_Name);
	if (AllCamera.contains(str) == true) {
		MainCamera = AllCamera[str];
	}
	return;
}

void GameEngine::CameraUpdate()
{
	if (MainCamera == nullptr) {
		EngineDebug::Error("카메라 없는데 업데이트중");
		exit(EXIT_FAILURE);
		return;
	}
	else {
		ViewMatrix = MainCamera->GetTransform().GetInverseMatrix();
	}

}

std::shared_ptr<Actor> GameEngine::SpawnActor()
{
	std::shared_ptr<Actor> AActor = std::make_shared<Actor>();
	AllActor.push_back(AActor);
	return AActor;
}



