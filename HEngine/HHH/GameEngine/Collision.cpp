#include "HDirect/EngineHlsl.h"
#include "HDirect/EngineScene.h"
#include "HDirect/GraphicsEngine.h"
#include "EngineHelper/AllStruct.h"
#include "EngineHelper/HString.h"
#include "HDirect/EngineAnimatinSkeleton.h"
#include "EngineHelper/EngineNamespace.h"
#include "Collision.h"
#include "Actor.h"
#include "Level.h"

Collision::Collision()
{
	SetScene("boxcollision");
	SetHlsl("Default");
}

Collision::~Collision()
{
	CollisionScene = nullptr;
	Owner = nullptr;
	Hlsl = nullptr;
	CollisionCheckFunc.clear();
}

void Collision::CollisionRender(float _DeltaTime)
{
	for (std::pair<const std::string, std::shared_ptr<MH>>& mesh : CollisionScene->Meshs) {
		GraphicsEngine::get()->UpdateConstantBuffer(mesh.second->MeshMatrix, Cbuffer::MESH);
		GraphicsEngine::get()->CollisionRender(Hlsl, mesh.second.get());
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

void Collision::SetType(CollisionType _Type)
{
	std::shared_ptr<Collision> self = shared_from_this();
	World->Collisions[static_cast<int>(ColType)].remove(self);
	World->Collisions[static_cast<int>(_Type)].push_back(self);
}

std::list<std::shared_ptr<Collision>>& Collision::GetCollisionList(CollisionType _Type)
{
	return World->Collisions[static_cast<int>(_Type)];
}

void Collision::AddLocation(float _x, float _y, float _z)
{
	CollisionTransform.AddLocation(_x, _y, _z);
}

void Collision::AddScale(float _x, float _y, float _z)
{
	CollisionTransform.AddScale(_x, _y, _z);
}

void Collision::AddRotation(float _x, float _y, float _z)
{
	CollisionTransform.AddRotation(_x, _y, _z);
}

void Collision::SetLocation(float _x, float _y, float _z)
{
	CollisionTransform.SetLocation(_x, _y, _z);
}

void Collision::SetScale(float _x, float _y, float _z)
{
	CollisionTransform.SetScale(XMFLOAT3(_x, _y, _z));
}

void Collision::SetRotation(float _x, float _y, float _z)
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

const EngineTransform& Collision::GetTransform()
{
	if (Owner == nullptr) {
		assert(false);
	}
	const EngineTransform& Transform = Owner->GetTransform();
	const EngineTransform& CombineTransform = CollisionTransform.CombineWithParent(Transform);
	return CombineTransform;
}

void Collision::CollisionCheck()
{
	for (std::function<void(void)> func : CollisionCheckFunc) {
		func();
	}
}



void Collision::AddFunction(std::function<void(void)> _Fun)
{
	CollisionCheckFunc.push_back(_Fun);
}
