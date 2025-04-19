#include "WoodenBox.h"

WoodenBox::WoodenBox() 
{
}

WoodenBox::~WoodenBox() 
{
}

void WoodenBox::BeginPlay()
{
	Actor::BeginPlay();
	SetHlsl("default");
	SetScene("woodenbox");
	SetActorScale(0.1f, 0.1f, 0.1f);

}

void WoodenBox::Tick(float _deltatime)
{
	Actor::Tick(_deltatime);

	//GraphicsEngine::get()->UpdateConstantBuffer(outBoneMatrices, "MainPlayer");

}
