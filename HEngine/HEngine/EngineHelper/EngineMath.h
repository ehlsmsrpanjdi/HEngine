#pragma once

struct Int2D {
public:
	int X = 0;
	int Y = 0;
	Int2D() {
		X = 0;
		Y = 0;
	}

	Int2D(int x, int y) {
		X = x;
		Y = y;
	}
	Int2D(const Int2D& int2d) {
		X = int2d.X;
		Y = int2d.Y;
	}

	Int2D operator+=(const Int2D& _int2d) {
		X += _int2d.X;
		Y += _int2d.Y;
		return *this;
	}
};

struct float2D {
public:
	float X = 0;
	float Y = 0;
	float2D() {
		X = 0;
		Y = 0;
	}

	float2D(float x, float y) {
		X = x;
		Y = y;
	}
	float2D(const float2D& _float2D) {
		X = _float2D.X;
		Y = _float2D.Y;
	}

	float2D operator+=(const float2D& _float2D) {
		X += _float2D.X;
		Y += _float2D.Y;
		return *this;
	}
};

struct float4D {
public:
	float X = 0;
	float Y = 0;
	float Z = 0;
	float W = 0;
	float4D() {
		X = 0;
		Y = 0;
		Z = 0;
		W = 0;
	}

	float4D(float x, float y, float z, float w) {
		X = x;
		Y = y;
	}
	float4D(const float4D& _float4d) {
		X = _float4d.X;
		Y = _float4d.Y;
		Z = _float4d.Z;
		W = _float4d.W;
	}

	float4D operator+=(const float4D& _float4d) {
		X += _float4d.X;
		Y += _float4d.Y;
		Z += _float4d.Z;
		W += _float4d.W;
		return *this;
	}
};