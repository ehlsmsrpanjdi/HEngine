#include "Actor.h"
#include "HDirect/EngineMaterial.h"
#include "HDirect/EngineMesh.h"
#include "HDirect/GraphicsEngine.h"
#include "EngineHelper/AllStruct.h"


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

void Actor::SetMesh(std::string _str)
{
	Mesh = GraphicsEngine::get()->GetMesh(_str);
}

void Actor::SetMaterial(std::string _str)
{
	Material = GraphicsEngine::get()->GetMaterial(_str);
}

EngineTransform Actor::GetTransform()
{
	return ActorTransform;
}
