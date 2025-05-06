#pragma once
#include <vector>
#include <memory>
// Ό³Έν :
class AllConstantBufferRes
{
public:
	// constrcuter destructer
	AllConstantBufferRes();
	~AllConstantBufferRes();

	// delete Function
	AllConstantBufferRes(const AllConstantBufferRes& _Other) = delete;
	AllConstantBufferRes(AllConstantBufferRes&& _Other) noexcept = delete;
	AllConstantBufferRes& operator=(const AllConstantBufferRes& _Other) = delete;
	AllConstantBufferRes& operator=(AllConstantBufferRes&& _Other) noexcept = delete;

	void Init();
	
	void CreateAllBuffer(std::shared_ptr<class GraphicDevice> m_Device);
protected:

private:
	std::vector<std::shared_ptr<class ConstantBufferResource>> CBufferRes;
};

