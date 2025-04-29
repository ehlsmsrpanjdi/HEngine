#include "MainPlayer.h"
#include "HDirect/GraphicsEngine.h"
#include "HDirect/EngineScene.h"
#include "HDirect/EngineAnimatinSkeleton.h"
#include "Collision.h"
#include "memory"
#include "EngineHelper/EngineTransform.h"

MainPlayer::MainPlayer() 
{
}

MainPlayer::~MainPlayer() 
{
}

void MainPlayer::BeginPlay()
{
	Actor::BeginPlay();

	SetHlsl("default");
	SetScene("character");

	SetAnimation("dance");
	IsAnimation = true;

	Name = "Player";
	SetActorScale(0.1f, 0.1f, 0.1f);

	Collision* col = CreateCollision(CollisionType::Player);
	col->AddFunction([col]() {
		std::list<std::shared_ptr<Collision>> list = col->GetCollisionList(CollisionType::Normal);
		for (std::shared_ptr<Collision> collision : list) {
			if (col->GetTransform().SphereCollision(collision->GetTransform())) {
				EngineTransform A = col->GetTransform();
				EngineTransform B = collision->GetTransform();
				printf("Collision\n");
			}
			else {
				int a = 0;
			}
		}
		});
}

void MainPlayer::Tick(float _deltatime)
{
	Actor::Tick(_deltatime);
	//AddActorLocation(10.f * _deltatime);
}

