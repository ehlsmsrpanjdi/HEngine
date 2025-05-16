#include "MainPlayer.h"
#include "HDirect/GraphicsEngine.h"
#include "HDirect/EngineScene.h"
#include "HDirect/EngineAnimatinSkeleton.h"
#include "Collision.h"
#include "memory"
#include "EngineHelper/EngineTransform.h"
#include <HDirect/EngineHlslResource.h>

MainPlayer::MainPlayer()
{
}

MainPlayer::~MainPlayer()
{
}

void MainPlayer::BeginPlay()
{
	Actor::BeginPlay();

	SetHlsl(HlslNamespace::NoneAnimation);
	SetScene("character");
	Name = "Test";

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

void MainPlayer::Tick(float _deltatime)
{
	Actor::Tick(_deltatime);
	//AddActorLocation(10.f * _deltatime);
}

