#include "Actor.h"
#include "HDirect/EngineHlsl.h"
#include "HDirect/EngineScene.h"
#include "HDirect/GraphicsEngine.h"
#include "EngineHelper/AllStruct.h"
#include "EngineHelper/HString.h"
#include "HDirect/EngineAnimatinSkeleton.h"
#include "HDirect/ConstantBufferStruct.h"
#include "Collision.h"
#include "Level.h"
#include "HDirect/ConstantBufferResource.h"
#include "HDirect/EngineHlslResource.h"
#include "HDirect/EngineSamplerResource.h"

Actor::Actor()
{
}

Actor::~Actor()
{
	Hlsl = nullptr;
	ActorScene = nullptr;
	World = nullptr;
	SeletedFrame = nullptr;
}

void Actor::BeginPlay()
{
	Sampler = EngineSamplerResource::GetResource(SamplerNameSpace::DEFAULT)->GetSampler();
}

void Actor::Tick(float _DeltaTime)
{

}

void Actor::Render(float _DeltaTime)
{
	if (IsAnimation == true) {
		CurrentAnimTime += _DeltaTime;

		while (CurrentAnimTime > EndAnimTime) {
			CurrentAnimTime -= EndAnimTime;
		}

		ActorScene->AnimSkeleton->EvaluateAnimation(CurrentAnimTime, SeletedFrame, outBoneMatrices);

		XMMATRIX* Data = outBoneMatrices.data();

		ConstantBufferResource::UpdateConstantBuffer(static_cast<void*>(Data), Cbuffer::ANI);
		XMMATRIX MashMatrix = DirectX::XMMatrixIdentity();
		ConstantBufferResource::UpdateConstantBuffer(static_cast<void*>(&MashMatrix), Cbuffer::MESH);

	}
	else {
		for (size_t i = 0; i < outBoneMatrices.size(); ++i)
		{
			outBoneMatrices[i] = DirectX::XMMatrixIdentity();
		}
	}

	for (std::pair<const std::string, std::shared_ptr<MH>>& mesh : ActorScene->Meshs) {
		ConstantBufferResource::UpdateConstantBuffer(static_cast<void*>(&mesh.second->MeshMatrix), Cbuffer::MESH);
		GraphicsEngine::get()->Render(Hlsl, mesh.second.get(), Sampler);
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

	Hlsl = EngineHlslResource::GetResource(str)->GetHlsl().get();
}

void Actor::SetAnimation(std::string_view _str)
{
	SeletedFrame = ActorScene->AnimSkeleton->GetKeyFrame(_str);
	EndAnimTime = ActorScene->AnimSkeleton->GetEndTime(_str);
	CurrentAnimTime = 0.f;
}

Level* Actor::GetWorld()
{
	if (World != nullptr) {
		return World;
	}
	else {
		assert(false);
		return nullptr;
	}
	
}

void Actor::SetRoot(Actor* _Act)
{
	if (_Act == nullptr) {
		assert(false);
	}
	RootActor = _Act;
}

Actor* Actor::GetRoot()
{
	if (RootActor == nullptr) {
		return nullptr;
	}
	else {
		return RootActor;
	}
}


Collision* Actor::CreateCollision(CollisionType _Type)
{
	Level* Lv = GetWorld();
	if (Lv == nullptr) {
		assert(false);
		return nullptr;
	}

	int type = static_cast<int>(_Type);

	std::shared_ptr<Collision> Col = std::make_shared<Collision>();
	Col->SetOwner(this);
	Col->SetCollisionType(_Type);
	Col->SetWorld(Lv);
	Lv->Collisions[type].push_back(Col);

	return Col.get();
}

const std::shared_ptr<FScene> Actor::GetScene() const
{
	return ActorScene;
}


const EngineTransform& Actor::GetTransform()
{
	if (RootActor != nullptr) {
		const EngineTransform& parentTransform = RootActor->GetTransform();
		const EngineTransform& Result = this->ActorTransform.CombineWithParent(parentTransform);
		return Result;
	}
	return ActorTransform;
}

void Actor::SetWorld(Level* _Lv)
{
	if (_Lv == nullptr) {
		assert(false);
	}
	World = _Lv;
}
