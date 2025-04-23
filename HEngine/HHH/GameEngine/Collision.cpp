#include "HDirect/EngineHlsl.h"
#include "HDirect/EngineScene.h"
#include "HDirect/GraphicsEngine.h"
#include "EngineHelper/AllStruct.h"
#include "EngineHelper/HString.h"
#include "HDirect/EngineAnimatinSkeleton.h"
#include "EngineHelper/EngineNamespace.h"
#include "Collision.h"

Collision::Collision()
{
	SetScene("Boxcollision");
	SetHlsl("Default");
	SetActorScale(0.1f, 0.1f, 0.1f);
}

Collision::~Collision()
{
	CollisionScene = nullptr;
	Owner = nullptr;
	Hlsl = nullptr;
}

void Collision::CollisionRender(float _DeltaTime)
{
	for (std::pair<const std::string, std::shared_ptr<MH>>& mesh : CollisionScene->Meshs) {
		GraphicsEngine::get()->UpdateConstantBuffer(mesh.second->MeshMatrix, Cbuffer::MESH);
		GraphicsEngine::get()->Render(Hlsl, mesh.second.get());
	}
}

void Collision::SetScene(std::string_view _str)
{
	std::string str = HString::Upper(_str.data());
	CollisionScene = GraphicsEngine::get()->GetScene(str);
}

void Collision::SetHlsl(std::string_view _str)
{
	std::string str = HString::Upper(_str.data());
	Hlsl = GraphicsEngine::get()->GetHlsl(str);
}

void Collision::SetCollisionType(CollisionType _Type)
{
	ColType = _Type;
}

void Collision::SetOwner(Actor* _Actor)
{
	if (_Actor == nullptr) {
		assert(false);
	}
	Owner = _Actor;
}

std::shared_ptr<FScene>Collision::GetScene()
{
	if (CollisionScene == nullptr) {
		assert(false);
	}
	return CollisionScene;
}

void Collision::AddActorLocation(float _x, float _y, float _z)
{
	CollisionTransform.AddLocation(_x, _y, _z);
}

void Collision::AddActorScale(float _x, float _y, float _z)
{
	CollisionTransform.AddScale(_x, _y, _z);
}

void Collision::AddActorRotation(float _x, float _y, float _z)
{
	CollisionTransform.AddRotation(_x, _y, _z);
}

void Collision::SetActorLocation(float _x, float _y, float _z)
{
	CollisionTransform.SetLocation(_x, _y, _z);
}

void Collision::SetActorScale(float _x, float _y, float _z)
{
	CollisionTransform.SetScale(XMFLOAT3(_x, _y, _z));
}

void Collision::SetActorRotation(float _x, float _y, float _z)
{
	CollisionTransform.SetRotation(XMFLOAT4(_x, _y, _z, 1.f));
}

void Collision::Move(float _x, float _y, float _z)
{
	CollisionTransform.Move(_x, _y, _z);
}

void Collision::Rotate(float _x, float _y, float _z)
{
	CollisionTransform.Rotate(_x, _y, _z);
}

EngineTransform& Collision::GetTransform()
{
	return CollisionTransform;
}
