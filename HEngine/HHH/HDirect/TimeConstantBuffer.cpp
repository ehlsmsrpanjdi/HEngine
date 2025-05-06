#include "TimeConstantBuffer.h"

TimeConstantBuffer::TimeConstantBuffer() 
{
}

TimeConstantBuffer::~TimeConstantBuffer() 
{
}

void TimeConstantBuffer::Init()
{
	BufferSize = sizeof(TIME);
	BufferName = Cbuffer::TIME;
}
