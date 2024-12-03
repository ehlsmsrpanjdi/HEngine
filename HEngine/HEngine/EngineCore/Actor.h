#pragma once
#include "BaseObject.h"


class Actor : public BaseObject {
public:
	friend class EngineCore;

	Actor() {

	}

	virtual void BeginPlay();

	virtual void Tick(float _deltatime) override;

};