#pragma once
#include "EngineHelper/EngineTransform.h"
#include "BaseObject.h"
class EngineImage;
class Actor;

class Renderer : public BaseObject{
	friend Actor;
public:
	Renderer() {

	}

	~Renderer();

	void SetImage(EngineImage* _Image);
	void SetOwner(Actor* _Actor);
	void Rendering();

	Actor* GetOwner() {
		if (Owner != nullptr) {
			return Owner;
		}
		CodeError;
		return nullptr;
	}

private:
	EngineImage* RImage = nullptr;
	Actor* Owner = nullptr;
};