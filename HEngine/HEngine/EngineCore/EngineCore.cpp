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
	HBITMAP bmp = CreateCompatibleBitmap(MainWindow.GetHDC(), MainWindow.WindowSize.X, MainWindow.WindowSize.Y);
	HBITMAP prev = (HBITMAP)::SelectObject(MainWindow.BackBuffer, bmp);
	DeleteObject(prev);
	ImageInit::ImageIniteralize(MainCore->GetWindow().GetHDC(), MainCore->GetWindow().GetBack());
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


	if (CollisionRendering == true) {
		for (std::pair<const int, std::unordered_set<EngineCollision*>>& pa : EngineCollision::Collisions) {
			for (EngineCollision* Collision : pa.second) {
				Collision->CollisionDraw(GetWindow().GetHDC());
			}
		}
	}


	DoubleBuffering();
}

void EngineCore::DoubleBuffering(){
	StretchBlt(GetWindow().GetHDC(),        // 대상 DC			/bitblt 안쓴 이유는 bitblt은 사진 이미지 크기 조절이 안댐
		0, 0,   // 대상 위치
		EngineWindow::WindowSize.X, EngineWindow::WindowSize.X,   // 출력할 이미지 크기
		GetWindow().GetBack(),      // 원본 DC
		0, 0,       // 원본 위치
		EngineWindow::WindowSize.X, EngineWindow::WindowSize.X,   // 원본 이미지 크기
		SRCCOPY);   // 복사 방식

	//BitBlt(GetWindow().GetHDC(), 0, 0, EngineWindow::WindowSize.X, EngineWindow::WindowSize.X, GetWindow().GetBack(), 0, 0, SRCCOPY);
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