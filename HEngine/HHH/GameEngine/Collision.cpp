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
}

Collision::~Collision() 
{
	CollisionScene = nullptr;
	Owner = nullptr;
	Hlsl = nullptr;
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
