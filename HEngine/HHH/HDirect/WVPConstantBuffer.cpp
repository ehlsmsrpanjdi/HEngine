#include "WVPConstantBuffer.h"


WVPConstantBuffer::WVPConstantBuffer() 
{
}

WVPConstantBuffer::~WVPConstantBuffer() 
{
}

void WVPConstantBuffer::Init()
{
	BufferSize = sizeof(WVP);
	BufferName = Cbuffer::WVP;
}

