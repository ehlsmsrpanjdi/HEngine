#include "AnimationConstantBuffer.h"

AnimationConstantBuffer::AnimationConstantBuffer() 
{
}

AnimationConstantBuffer::~AnimationConstantBuffer() 
{
}

void AnimationConstantBuffer::Init()
{
	BufferSize = sizeof(ANI);
	BufferName = Cbuffer::ANI;
}
