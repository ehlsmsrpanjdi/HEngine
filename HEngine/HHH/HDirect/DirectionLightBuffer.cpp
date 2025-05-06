#include "DirectionLightBuffer.h"

DirectionLightBuffer::DirectionLightBuffer() 
{
}

DirectionLightBuffer::~DirectionLightBuffer() 
{
}

void DirectionLightBuffer::Init()
{
	BufferSize = sizeof(LightBuffer);
	BufferName = Cbuffer::LIGHT;
}
