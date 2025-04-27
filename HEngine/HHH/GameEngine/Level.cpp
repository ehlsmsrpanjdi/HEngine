#include "Level.h"
#include <HDirect/GraphicsEngine.h>
#include <EngineHelper/EngineNamespace.h>
#include <GameEngine/GameEngine.h>
#include "Actor.h"
#include "Collision.h"

Level::Level() 
{
}

Level::~Level() 
{
	AllActor.clear();
	Collisions.clear();
}

void Level::BeginPlay()
{
	PerseMatrix = GameEngine::get()->PerseMatrix;
}

void Level::EndPlay()
{
}

void Level::Tick(float _DeltaTime)
{
	for (std::shared_ptr<Actor> Act : AllActor) {
		Act->Tick(_DeltaTime);
	}
}


void Level::Render(float _DeltaTime)
{
	CameraMatrixUpdate(_DeltaTime);
	for (std::shared_ptr<Actor> Act : AllActor) {
		if (Act->GetScene() == nullptr) {
			continue;
		}
		WorldMatrix = Act->GetTransform().GetWorldMatrix();
		WVP = WorldMatrix * ViewMatrix * PerseMatrix;
		GraphicsEngine::get()->UpdateConstantBuffer(WVP, Cbuffer::WVP);
		Act->Render(_DeltaTime);
	}
}

void Level::CollisionRender(float _DeltaTime)
{
	CameraMatrixUpdate(_DeltaTime);
	for (auto& [index, element] : Collisions) {
		for (std::shared_ptr<Collision> Col : element) {
			if (Col->GetScene() == nullptr) {
				continue;
			}
			WorldMatrix = Col->GetTransform().GetWorldMatrix();
			WVP = WorldMatrix * ViewMatrix * PerseMatrix;
			GraphicsEngine::get()->UpdateConstantBuffer(WVP, Cbuffer::WVP);
			Col->CollisionRender(_DeltaTime);
		}
	}


}

void Level::SetMainCamera(std::string _Name)
{
	std::string str = HString::Upper(_Name);
	if (AllCamera.contains(str) == true) {
		MainCamera = AllCamera[str].get();
	}
	return;
}

void Level::StartLevel()
{
}

void Level::EndLevel()
{
}

void Level::CameraMatrixUpdate(float _DeltaTime)
{
	if (MainCamera == nullptr) {
		//EngineDebug::Error("카메라 없는데 업데이트중");
		assert(false);
		return;
	}
	else {
		EngineTransform transform = MainCamera->GetTransform();
		ViewMatrix = transform.GetInverseMatrix();
	}

}



