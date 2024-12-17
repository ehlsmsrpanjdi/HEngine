#include "pch.h"
#include "EngineCore.h"
#include "EngineHelper/EngineTime.h"
#include "Actor.h"
#include "EngineHelper/EngineImage.h"
#include "EngineHelper/EngineResource.h"
#include "EngineHelper/EngineKey.h"
#include "EngineCollision.h"
#include "Level.h"
EngineCore* EngineCore::MainCore = nullptr;

EngineCore::EngineCore(HINSTANCE _inst, int _x, int _y) {
	EngineKey::InitKey();
	TimeManager = new EngineTime();
	MainWindow.SetWindowSize(_x, _y);
	MainWindow.WindowRegister(_inst);
	MainWindow.WindowOpen();
}

void EngineCore::EngineStart() {
	MainCore = this;
	MainWindow.BackBuffer = CreateCompatibleDC(MainWindow.GetHDC());
	ImageInit::ImageIniteralize(MainCore->GetWindow().GetHDC());
	BeginPlay();
	MainWindow.WindowStart(std::bind(&EngineCore::EngineTick, this), std::bind(&EngineCore::EngineEnd, this));
}

void EngineCore::BeginPlay() {

}

void EngineCore::EngineTick() {
	float DeltaTime = static_cast<float>(TimeManager->UpdateDeltaTime());
	EngineKey::KeyCheck();

	for (Level* Lev : Levels) {
		Lev->Tick(DeltaTime);
	}

	//Rectangle(GetInst()->GetWindow().GetHDC(), 0, 0, GetInst()->GetWindow().WindowSize.X, GetInst()->GetWindow().WindowSize.Y);

	for (Level* Lev : Levels) {
		Lev->RenderTick(DeltaTime);
	}


	//if (CollisionRendering == true) {
	//	for (std::pair<const int, std::unordered_set<EngineCollision*>>& pa : EngineCollision::Collisions) {
	//		for (EngineCollision* Collision : pa.second) {
	//			Collision->CollisionDraw(GetWindow().GetHDC());
	//		}
	//	}
	//}
	DoubleBuffering();
}

void EngineCore::DoubleBuffering(){
	BitBlt(GetWindow().GetHDC(), 0, 0, EngineWindow::WindowSize.X, EngineWindow::WindowSize.X, GetWindow().GetBack(), 0, 0, SRCCOPY);
}
void EngineCore::EngineEnd() {
	for (Level* Lev: Levels) {
		delete Lev;
	}
	EngineResource::GetInst()->ReleaseResources();
	delete EngineResource::GetInst();
	ImageInit::ImageExit();
	delete TimeManager;
}