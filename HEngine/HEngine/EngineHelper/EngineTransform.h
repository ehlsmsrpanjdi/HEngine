#pragma once
#include "EngineMath.h"

class EngineTransform {
public:
	EngineTransform() {

	}

	EngineTransform(float2D _Scale, float2D _position) {
		Position = _position;
		Scale = _Scale;
	}

	EngineTransform(const EngineTransform& _Transform) {
		Position = _Transform.Position;
		Scale = _Transform.Scale;
	}

	void SetTransform(const EngineTransform& _transform) {
		Position = _transform.Position;
		Scale = _transform.Scale;
	}

	void SetTransform(float _x, float _y, float _z, float _w) {
		Position.X = _x;
		Position.Y = _y;
		Scale.X = _z;
		Scale.Y = _w;
	}

	void AddPosition(float2D _pos) {
		Position += _pos;
	}

	void AddPosition(float _x, float _y) {
		Position.X += _x;
		Position.Y += _y;
	}

	void AddScale(float2D _scale) {
		Scale += _scale;
	}

	void AddScale(float _x, float _y) {
		Scale.X += _x;
		Scale.Y += _y;
	}



	float2D Position = { 0.f,0.f };
	float2D Scale = { 1.f,1.f };
};