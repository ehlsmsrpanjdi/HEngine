#pragma once
#include "EngineCore/EngineCore.h"


class SMCore : public EngineCore {
public:
	SMCore(HINSTANCE _inst, int _x, int _y)
		:EngineCore(_inst, _x, _y)
	{

	}

	void EngineStart() override;

	void BeginPlay() override;

protected:
	void EngineTick() override;
	void EngineEnd() override;

};