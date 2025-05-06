#include "GlobalMeshMatrixConstantBuffer.h"

GlobalMeshMatrixConstantBuffer::GlobalMeshMatrixConstantBuffer() 
{
}

GlobalMeshMatrixConstantBuffer::~GlobalMeshMatrixConstantBuffer() 
{
}

void GlobalMeshMatrixConstantBuffer::Init()
{
	BufferSize = sizeof(MESH);
	BufferName = Cbuffer::MESH;
}
