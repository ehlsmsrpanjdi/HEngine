#include "MainPlayer.h"
#include "HDirect/GraphicsEngine.h"
#include "HDirect/EngineScene.h"
#include "HDirect/EngineAnimatinSkeleton.h"
#include "Collision.h"
#include "memory"
#include "EngineHelper/EngineTransform.h"
#include <HDirect/EngineHlslResource.h>
#include "EngineHelper/EngineKey.h"

MainPlayer::MainPlayer()
{
}

MainPlayer::~MainPlayer()
{
}

void MainPlayer::BeginPlay()
{
	Actor::BeginPlay();

	SetHlsl(HlslNamespace::Default);
	SetScene("character");
	SetPlayerAnimation(IdleAnimationName);
	Name = "Test";

	SetActorScale(0.1f, 0.1f, 0.1f);

	//Collision* col = CreateCollision(CollisionType::Player);
	//col->AddFunction([col]() {
	//	std::list<std::shared_ptr<Collision>> list = col->GetCollisionList(CollisionType::Normal);
	//	for (std::shared_ptr<Collision> collision : list) {
	//		EngineTransform A = col->GetTransform();
	//		EngineTransform B = collision->GetTransform();
	//		//if (EngineTransform::SphereCollision(A, B)) {
	//		//	printf("Collision\n");
	//		//}
	//		//else {
	//		//	int a = 0;
	//		//}
	//		if (EngineTransform::OBB(A, B)) {
	//			printf("CollisionOBB\n");
	//		}
	//		else {
	//			int a = 0;
	//		}

	//	}
	//	});
}

void MainPlayer::Tick(float _Deltatime)
{
	Actor::Tick(_Deltatime);
	bool isinput = false;
	if (EngineKey::IsPressed('W')) {
		ForwardSpeed += 10.f * _Deltatime;
		isinput = true;
	}
	if (EngineKey::IsPressed('S')) {
		ForwardSpeed -= 10.f * _Deltatime;
		isinput = true;
	}
	if (EngineKey::IsPressed('A')) {
		LeftSpeed -= 10.f * _Deltatime;
		isinput = true;
	}
	if (EngineKey::IsPressed('D')) {
		LeftSpeed += 10.f * _Deltatime;
		isinput = true;
	}

	if (isinput == false) {
		CalculateFirction();
	}

	if (LeftSpeed != 0 || ForwardSpeed != 0) {
		SetPlayerAnimation(RunAnimationName);
	}
	else {
		SetPlayerAnimation(IdleAnimationName);
	}


	AddActorLocation(LeftSpeed * _Deltatime, 0.f, ForwardSpeed * _Deltatime);
}

void MainPlayer::CalculateFirction() {
	if (LeftSpeed != 0) {
		LeftSpeed /= 2;
		if (std::abs(LeftSpeed) < 1.f) {
			LeftSpeed = 0;
		}
	}

	if (ForwardSpeed != 0) {
		ForwardSpeed /= 2;
		if (std::abs(ForwardSpeed) < 1.f) {
			ForwardSpeed = 0;
		}
	}
}

void MainPlayer::SetPlayerAnimation(const std::string& _string)
{
	if (_string == CurrentAnimationName) {
		return;
	}
	SetAnimation(_string);
	CurrentAnimationName = _string;
}
