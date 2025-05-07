#include "AllConstantBufferRes.h"
#include "AnimationConstantBuffer.h"
#include "GlobalMeshMatrixConstantBuffer.h"
#include "TimeConstantBuffer.h"
#include "WVPConstantBuffer.h"
#include "GraphicDevice.h"
#include "ConstantBufferResource.h"
#include "DirectionLightBuffer.h"


AllConstantBufferRes::AllConstantBufferRes()
{
}

AllConstantBufferRes::~AllConstantBufferRes()
{
	CBufferRes.clear();
}

void AllConstantBufferRes::Init()
{
	CBufferRes.push_back(std::make_shared<AnimationConstantBuffer>());
	CBufferRes.push_back(std::make_shared<GlobalMeshMatrixConstantBuffer>());
	CBufferRes.push_back(std::make_shared<TimeConstantBuffer>());
	CBufferRes.push_back(std::make_shared<WVPConstantBuffer>());
	CBufferRes.push_back(std::make_shared<DirectionLightBuffer>());

	for (auto& res : CBufferRes) {
		res->Init();
	}
}

void AllConstantBufferRes::CreateAllBuffer(std::shared_ptr<class GraphicDevice> m_Device)
{
	for (auto& res : CBufferRes) {
		res->CreateBuffer(m_Device);
	}
}

