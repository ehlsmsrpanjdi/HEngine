#include "Actor.h"
#include "HDirect/EngineHlsl.h"
#include "HDirect/EngineScene.h"
#include "HDirect/GraphicsEngine.h"
#include "EngineHelper/AllStruct.h"
#include "EngineHelper/HString.h"
#include "HDirect/EngineAnimatinSkeleton.h"


Actor::Actor()
{
}

Actor::~Actor()
{
	Hlsl = nullptr;
	ActorScene = nullptr;
}

void Actor::BeginPlay()
{
}

void Actor::Tick(float _DeltaTime)
{
	if (IsAnimation == true) {
		ActorScene->AnimSkeleton->EvaluateAnimation(_DeltaTime, outBoneMatrices);
	}
}

void Actor::Render()
{
	for (std::pair<const std::string, std::shared_ptr<MH>>& mesh : ActorScene->Meshs) {
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

void Actor::SetScene(std::string_view _str)
{
	std::string str = HString::Upper(_str.data());
	ActorScene = GraphicsEngine::get()->GetScene(str);
	//Meshs = &mesh;
}

void Actor::SetHlsl(std::string_view _str)
{
	std::string str = HString::Upper(_str.data());
	Hlsl = GraphicsEngine::get()->GetHlsl(str);
}

void Actor::SetAnimation(std::string_view _str)
{
	ActorScene->AnimSkeleton->SetAnimation(_str);
}

void Actor::SetAnimationTemp()
{
	ActorScene->AnimSkeleton->SetAnimationTemp();
}

EngineTransform Actor::GetTransform()
{
	return ActorTransform;
}
