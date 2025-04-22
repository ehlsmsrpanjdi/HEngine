#include "Actor.h"
#include "HDirect/EngineHlsl.h"
#include "HDirect/EngineScene.h"
#include "HDirect/GraphicsEngine.h"
#include "EngineHelper/AllStruct.h"
#include "EngineHelper/HString.h"
#include "HDirect/EngineAnimatinSkeleton.h"
#include "EngineHelper/EngineNamespace.h"
#include "Collision.h"
#include "Level.h"

Actor::Actor()
{
}

Actor::~Actor()
{
	Hlsl = nullptr;
	ActorScene = nullptr;
	World = nullptr;
}

void Actor::BeginPlay()
{
}

void Actor::Tick(float _DeltaTime)
{

}

void Actor::Render(float _DeltaTime)
{
	if (IsAnimation == true) {
		ActorScene->AnimSkeleton->EvaluateAnimation(_DeltaTime, outBoneMatrices);

		GraphicsEngine::get()->UpdateConstantBuffer(outBoneMatrices, Cbuffer::ANI);
		GraphicsEngine::get()->UpdateConstantBuffer(DirectX::XMMatrixIdentity(), Cbuffer::MESH);
	}
	else {
		for (size_t i = 0; i < outBoneMatrices.size(); ++i)
		{
			outBoneMatrices[i] = DirectX::XMMatrixIdentity();
		}
	}

	for (std::pair<const std::string, std::shared_ptr<MH>>& mesh : ActorScene->Meshs) {

		GraphicsEngine::get()->UpdateConstantBuffer(mesh.second->MeshMatrix, Cbuffer::MESH);
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

Level* Actor::GetWorld()
{
	if (World != nullptr) {
	return World;
	}
	else {
		assert(false);
	}
}


Collision* Actor::CreateCollision(CollisionType _Type)
{
	Level* Lv = GetWorld();
	if (Lv == nullptr) {
		assert(false);
	}

	int type = static_cast<int>(_Type);

	std::shared_ptr<Collision> Col = std::make_shared<Collision>();
	Col->SetOwner(this);
	Col->SetCollisionType(_Type);
	Lv->Collisions[type].push_back(Col);

	return Col.get();
}

const std::shared_ptr<FScene> Actor::GetScene() const
{
	return ActorScene;
}


EngineTransform Actor::GetTransform()
{
	return ActorTransform;
}

void Actor::SetWorld(Level* _Lv)
{
	if (_Lv == nullptr) {
		assert(false);
	}
	World = _Lv;
}
