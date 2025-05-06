#include "Level.h"
#include <HDirect/GraphicsEngine.h>
#include "HDirect/ConstantBufferStruct.h"
#include <GameEngine/GameEngine.h>
#include "Actor.h"
#include "Collision.h"
#include "LightActor.h"
#include "HDirect/ConstantBufferResource.h"

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

	CollisionCheck();
}


void Level::Render(float _DeltaTime)
{
	ConstantBufferResource::SetVSConstantBuffer(0, Cbuffer::WVP);
	ConstantBufferResource::SetVSConstantBuffer(1, Cbuffer::ANI);
	ConstantBufferResource::SetVSConstantBuffer(2, Cbuffer::MESH);

	ConstantBufferResource::SetPSConstantBuffer(0, Cbuffer::WVP);
	ConstantBufferResource::SetPSConstantBuffer(1, Cbuffer::ANI);
	ConstantBufferResource::SetPSConstantBuffer(2, Cbuffer::MESH);


	CameraMatrixUpdate(_DeltaTime);
	for (std::shared_ptr<Actor> Act : AllActor) {
		if (Act->GetScene() == nullptr) {
			continue;
		}
		WorldMatrix = Act->GetTransform().GetWorldMatrix();
		WVPBuffer.WorldMatrix = WorldMatrix;
		WVPBuffer.ViewMatrix = ViewMatrix;
		WVPBuffer.ProjectionMatrix = PerseMatrix;
		WVPBuffer.WVPMatrix = WorldMatrix * ViewMatrix * PerseMatrix;
		WVP* Data = &WVPBuffer;

		ConstantBufferResource::UpdateConstantBuffer(static_cast<void*>(Data), Cbuffer::WVP);
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
			WVPBuffer.WorldMatrix = WorldMatrix;
			WVPBuffer.ViewMatrix = ViewMatrix;
			WVPBuffer.ProjectionMatrix = PerseMatrix;

			ConstantBufferResource::UpdateConstantBuffer(static_cast<void*>(&WVPBuffer), Cbuffer::WVP);
			Col->CollisionRender(_DeltaTime);
		}
	}


}

void Level::SetMainCamera(std::string_view _Name)
{
	std::string str = HString::Upper(_Name.data());
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

std::shared_ptr<class LightActor> Level::CreateLight(LightType _LightType)
{
	std::shared_ptr<class LightActor> Light = std::make_shared<class LightActor>();
	Light->SetWorld(this);
	Light->SetType(_LightType);
	Light->BeginPlay();
	AllActor.push_back(Light);
	return Light;
}


void Level::CollisionCheck()
{
	for (auto& [enumIndex, element] : Collisions) {
		for (std::shared_ptr<Collision> Col : element) {
			Col->CollisionCheck();
		}
	}
}



