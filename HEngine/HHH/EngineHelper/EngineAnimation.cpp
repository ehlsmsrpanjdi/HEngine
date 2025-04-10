#include "EngineAnimation.h"
#include "EngineTransform.h"

EngineAnimation::EngineAnimation() 
{
}

EngineAnimation::~EngineAnimation() 
{
}

void EngineAnimation::AddBoneKeyFrame(const std::string& boneName, int frame, const EngineTransform& transform)
{
}

void EngineAnimation::SetDuration(float startTime, float endTime)
{
	StartTime = startTime;
	EndTime = endTime;
}

void EngineAnimation::Update(float deltaTime)
{
}
