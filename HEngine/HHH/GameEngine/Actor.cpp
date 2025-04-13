#include "Actor.h"
#include "HDirect/EngineHlsl.h"
#include "HDirect/EngineScene.h"
#include "HDirect/GraphicsEngine.h"
#include "EngineHelper/AllStruct.h"
#include "EngineHelper/HString.h"


Actor::Actor()
{
}

Actor::~Actor()
{
	Hlsl = nullptr;
	Meshs = nullptr;
	Animator = nullptr;
}

void Actor::BeginPlay()
{
	if (Hlsl == nullptr) {
		assert(false);
	}

}

void Actor::Tick(float _DeltaTime)
{
}

void Actor::Render()
{
	for (std::pair<const std::string, std::shared_ptr<MH>>& mesh : *Meshs) {
		GraphicsEngine::get()->Render(Hlsl, mesh.second.get());
	}
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

void Actor::Move(float _x, float _y, float _z)
{
	ActorTransform.Move(_x, _y, _z);
}

void Actor::Rotate(float _x, float _y, float _z)
{
	ActorTransform.Rotate(_x, _y, _z);
}

void Actor::SetMesh(std::string_view _str)
{
	std::string str = HString::Upper(_str.data());
	std::unordered_map<std::string, std::shared_ptr<MH>>& mesh = GraphicsEngine::get()->GetMesh(str);
	Meshs = &mesh;
}

void Actor::SetHlsl(std::string_view _str)
{
	std::string str = HString::Upper(_str.data());
	Hlsl = GraphicsEngine::get()->GetHlsl(str);
}

EngineTransform Actor::GetTransform()
{
	return ActorTransform;
}
