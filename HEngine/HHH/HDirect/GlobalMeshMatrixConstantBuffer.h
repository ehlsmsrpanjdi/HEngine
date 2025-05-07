#pragma once
#include "ConstantBufferResource.h"
#include "ConstantBufferStruct.h"
#include "EngineResource.h"
// Ό³Έν :
class GlobalMeshMatrixConstantBuffer : public ConstantBufferResource
{
public:
	// constrcuter destructer
	GlobalMeshMatrixConstantBuffer();
	~GlobalMeshMatrixConstantBuffer();

	// delete Function
	GlobalMeshMatrixConstantBuffer(const GlobalMeshMatrixConstantBuffer& _Other) = delete;
	GlobalMeshMatrixConstantBuffer(GlobalMeshMatrixConstantBuffer&& _Other) noexcept = delete;
	GlobalMeshMatrixConstantBuffer& operator=(const GlobalMeshMatrixConstantBuffer& _Other) = delete;
	GlobalMeshMatrixConstantBuffer& operator=(GlobalMeshMatrixConstantBuffer&& _Other) noexcept = delete;


	void Init() override;
protected:

private:

};

