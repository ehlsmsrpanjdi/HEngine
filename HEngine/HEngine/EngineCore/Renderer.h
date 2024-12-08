#pragma once
#include "EngineHelper/EngineTransform.h"
class EngineImage;
class Actor;

class Renderer {
	friend Actor;
public:
	Renderer() {

	}

	~Renderer();

	void SetImage(EngineImage* _Image);
	void SetOwner(Actor* _Actor);
	void Rendering();
	void SetRenderSize(float2D _float2d) {
		RenderTransform.Scale = _float2d;
	}
	void SetRenderSize(float _x, float _y) {
		RenderTransform.Scale = { _x,_y };
	}

	EngineTransform GetTransform() {
		return RenderTransform;
	}
private:
	EngineImage* RImage = nullptr;
	Actor* Owner = nullptr;
	EngineTransform RenderTransform;
};