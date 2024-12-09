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

	virtual void Tick(float _deltatime) {

	}
	virtual void BeginPlay() {

	}

	void SetTransform(EngineTransform _Transform) {
		Transform = _Transform;
	}

	void SetLocation(float2D _Location) {
		Transform.Position = _Location;
	}

	void SetLocation(float _x, float _y){
		Transform.Position.X = _x;
		Transform.Position.Y = _y;
	}

	void AddLocation(float2D _Location) {
		Transform.Position += _Location;
	}

	void AddLocation(float _x, float _y){
		Transform.Position.X += _x;
		Transform.Position.Y += _y;
	}

	float2D GetScale() {
		return Transform.Scale;
	}

	EngineTransform GetTransform() {
		return Transform;
	}

	float2D GetLocation() {
		return Transform.Position;
	}

	void SetScale(float2D _Scale) {
		Transform.Scale = _Scale;
	}

	void SetScale(float _x, float _y) {
		Transform.Scale.X = _x;
		Transform.Scale.Y = _y;
	}

	void SetName(std::string_view _name) {
		ObjectName = _name;
	}

	std::string ObjectName;
	EngineTransform Transform;
};