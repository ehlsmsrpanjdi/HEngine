#pragma once
#include <iostream>
#include "EngineHelper/EngineTransform.h"

class BaseObject {
public:
	BaseObject() {

	}

	BaseObject(std::string_view _str)
		:ObjectName(_str)
	{

	}

	virtual void Tick(float _deltatime) = 0;
	virtual void BeginPlay() = 0;

	void SetTransform(EngineTransform _Transform) {
		Transform = _Transform;
	}

	void SetLocation(float2D _Location) {
		Transform.Position = _Location;
	}

	void SetScale(float2D _Scale) {
		Transform.Scale = _Scale;
	}

	void SetName(std::string_view _name) {
		ObjectName = _name;
	}

	std::string ObjectName;
	EngineTransform Transform;
};