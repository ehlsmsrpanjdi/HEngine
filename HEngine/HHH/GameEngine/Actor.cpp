#include "Actor.h"
#include "HDirect/EngineMaterial.h"
#include "HDirect/EngineMesh.h"
#include "HDirect/GraphicsEngine.h"
#include "EngineHelper/AllStruct.h"
#include "EngineHelper/HString.h"


Actor::Actor()
{
}

Actor::~Actor()
{
}

void Actor::BeginPlay()
{
	if (Material == nullptr) {
		assert(false);
	}
	if (Mesh == nullptr) {
		assert(false);
	}
}

void Actor::Tick(float _DeltaTime)
{
}

void Actor::Render()
{
	GraphicsEngine::get()->Render(Material, Mesh);
}

void Actor::AddActorLocation(float _x, float _y, float _z)
{
	ActorTransform.AddLocation(_x, _y, _z);
}

void Actor::AddActorScale(float _x, float _y, float _z)
{
	ActorTransform.AddScale(_x, _y, _z);
}

void Actor::AddActorRotation(float _x, float _y, float _z)
{
	ActorTransform.AddRotation(_x, _y, _z);
}

void Actor::SetActorLocation(float _x, float _y, float _z)
{
	ActorTransform.SetLocation(_x, _y, _z);
}

void Actor::SetActorScale(float _x, float _y, float _z)
{
	ActorTransform.SetScale(XMFLOAT3(_x, _y, _z));
}

void Actor::SetActorRotation(float _x, float _y, float _z)
{
	ActorTransform.SetRotation(XMFLOAT4(_x, _y, _z, 1.f));
}

void Actor::SetMesh(std::string_view _str)
{
	std::string str = HString::Upper(_str.data());
	Mesh = GraphicsEngine::get()->GetMesh(str);
}

void Actor::SetMaterial(std::string_view _str)
{
	std::string str = HString::Upper(_str.data());
	Material = GraphicsEngine::get()->GetMaterial(str);
}

EngineTransform Actor::GetTransform()
{
	return ActorTransform;
}
