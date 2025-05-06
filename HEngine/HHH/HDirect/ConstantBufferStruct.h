#pragma once
#include "DirectXMath.h"
#include "d3d11.h"
#include "iostream"

namespace Cbuffer {
	extern std::string WVP;
	extern std::string ANI;
	extern std::string MESH;
	extern std::string LIGHT;
	extern std::string TIME;
}

struct WVP {
	DirectX::XMMATRIX WorldMatrix = DirectX::XMMATRIX();
	DirectX::XMMATRIX ViewMatrix = DirectX::XMMATRIX();
	DirectX::XMMATRIX ProjectionMatrix = DirectX::XMMATRIX();
};

struct ANI {
	DirectX::XMMATRIX AnimationArr[100];
};

struct TIME {
	float Time = 0.0f;
	DirectX::XMFLOAT3 padding = DirectX::XMFLOAT3();
};

struct MESH {
	DirectX::XMMATRIX GlobalMeshMatrix;
};

struct LightBuffer
{
	DirectX::XMFLOAT3 DirectionOrPosition; // �� ���� (����Ʈ ����Ʈ�� ��� position)
	float Padding1;              // 16����Ʈ ����
	DirectX::XMFLOAT3 Color;     // ����
	float Intensity;             // ���
};