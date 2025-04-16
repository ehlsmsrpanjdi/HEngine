#pragma once

#include <DirectXMath.h>
#include "fbxsdk.h"
// Ό³Έν :
class EngineFbxMath
{
public:

	static DirectX::XMMATRIX ConvertFbxMatrixToXM(const FbxAMatrix& fbxMatrix)
	{
		return DirectX::XMMATRIX(
			static_cast<float>(fbxMatrix.Get(0, 0)), static_cast<float>(fbxMatrix.Get(0, 1)), static_cast<float>(fbxMatrix.Get(0, 2)), static_cast<float>(fbxMatrix.Get(0, 3)),
			static_cast<float>(fbxMatrix.Get(1, 0)), static_cast<float>(fbxMatrix.Get(1, 1)), static_cast<float>(fbxMatrix.Get(1, 2)), static_cast<float>(fbxMatrix.Get(1, 3)),
			static_cast<float>(fbxMatrix.Get(2, 0)), static_cast<float>(fbxMatrix.Get(2, 1)), static_cast<float>(fbxMatrix.Get(2, 2)), static_cast<float>(fbxMatrix.Get(2, 3)),
			static_cast<float>(fbxMatrix.Get(3, 0)), static_cast<float>(fbxMatrix.Get(3, 1)), static_cast<float>(fbxMatrix.Get(3, 2)), static_cast<float>(fbxMatrix.Get(3, 3))
		);
	}

   //static  DirectX::XMMATRIX XMMatrixLerp(const DirectX::XMMATRIX& m0, const DirectX::XMMATRIX& m1, float t)
   // {
   //     using namespace DirectX;

   //     XMFLOAT4X4 f4x4_0, f4x4_1;
   //     XMStoreFloat4x4(&f4x4_0, m0);
   //     XMStoreFloat4x4(&f4x4_1, m1);

   //     XMFLOAT4X4 result;
   //     for (int i = 0; i < 4; ++i)
   //     {
   //         result.m[i][0] =  DirectX::lerp(f4x4_0.m[i][0], f4x4_1.m[i][0], t);
   //         result.m[i][1] = lerp(f4x4_0.m[i][1], f4x4_1.m[i][1], t);
   //         result.m[i][2] = lerp(f4x4_0.m[i][2], f4x4_1.m[i][2], t);
   //         result.m[i][3] = lerp(f4x4_0.m[i][3], f4x4_1.m[i][3], t);
   //     }

   //     return XMLoadFloat4x4(&result);
   // }
};

