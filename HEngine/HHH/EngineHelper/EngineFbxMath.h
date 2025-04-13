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



};

