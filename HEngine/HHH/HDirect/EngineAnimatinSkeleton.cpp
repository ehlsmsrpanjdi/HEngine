#include "EngineAnimatinSkeleton.h"
#include "EngineHelper/AllStruct.h"
#include "EngineHelper/EngineFbxMath.h"
#include "DirectXMath.h"
#include "EngineHelper/EngineFSkeleton.h"
#include "EngineHelper/HString.h"

EngineAnimatinSkeleton::EngineAnimatinSkeleton()
{
}

EngineAnimatinSkeleton::~EngineAnimatinSkeleton()
{
	keyframesPerBoneMap.clear();
	Bones.clear();
}

void EngineAnimatinSkeleton::EvaluateAnimation(float time, std::vector<std::vector<struct KeyFrame>>* SeletedFrame, std::vector<DirectX::XMMATRIX>& outBoneMatrices)
{
	outBoneMatrices.clear();
	CurrentAnimatoinTime = time;

	if (SeletedFrame == nullptr) return;

	size_t boneCount = Bones.size();
	outBoneMatrices.resize(100, DirectX::XMMatrixIdentity());

	for (size_t boneIndex = 0; boneIndex < boneCount; ++boneIndex)
	{
		const std::vector<KeyFrame>& keyframes = (*SeletedFrame)[boneIndex];

		if (keyframes.empty())
		{
			outBoneMatrices[boneIndex] = DirectX::XMMatrixIdentity();
			continue;
		}

		// 첫 번째 또는 마지막 프레임이면 그대로
		if (CurrentAnimatoinTime <= keyframes.front().time)
		{
			const auto& kf = keyframes.front();
			DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&kf.position);
			DirectX::XMVECTOR rot = DirectX::XMLoadFloat4(&kf.rotation);
			outBoneMatrices[boneIndex] = DirectX::XMMatrixRotationQuaternion(rot) * DirectX::XMMatrixTranslationFromVector(pos);
			continue;
		}
		else if (CurrentAnimatoinTime >= keyframes.back().time)
		{
			const auto& kf = keyframes.back();
			DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&kf.position);
			DirectX::XMVECTOR rot = DirectX::XMLoadFloat4(&kf.rotation);
			outBoneMatrices[boneIndex] = DirectX::XMMatrixRotationQuaternion(rot) * DirectX::XMMatrixTranslationFromVector(pos);
			continue;
		}

		// 중간 프레임 보간
		for (size_t i = 0; i < keyframes.size() - 1; ++i)
		{
			const auto& kf1 = keyframes[i];
			const auto& kf2 = keyframes[i + 1];

			if (CurrentAnimatoinTime >= kf1.time && CurrentAnimatoinTime <= kf2.time)
			{
				float t = float((CurrentAnimatoinTime - kf1.time) / (kf2.time - kf1.time));

				auto pos1 = DirectX::XMLoadFloat3(&kf1.position);
				auto pos2 = DirectX::XMLoadFloat3(&kf2.position);
				auto lerpedPos = DirectX::XMVectorLerp(pos1, pos2, t);

				auto rot1 = DirectX::XMLoadFloat4(&kf1.rotation);
				auto rot2 = DirectX::XMLoadFloat4(&kf2.rotation);
				auto slerpedRot = DirectX::XMQuaternionSlerp(rot1, rot2, t);

				outBoneMatrices[boneIndex] = DirectX::XMMatrixRotationQuaternion(slerpedRot) * DirectX::XMMatrixTranslationFromVector(lerpedPos);
				break;
			}
		}
	}
	//outBoneMatrices[0] *= EngineFbxMath::ConvertFbxMatrixToXM(Bones[0].inverseGlobalBindPose);
	// 본의 부모 자식 관계 반영해서 로컬 → 월드로 누적 변환
	for (size_t i = 0; i < boneCount; ++i)
	{
		outBoneMatrices[i] = EngineFbxMath::ConvertFbxMatrixToXM(Bones[i].inverseGlobalBindPose) * outBoneMatrices[i];
	}
}

std::vector<std::vector<struct KeyFrame>>* EngineAnimatinSkeleton::GetKeyFrame(std::string_view _str)
{
	std::string str = HString::Upper(_str.data());
	return &keyframesPerBoneMap[str];
}

float EngineAnimatinSkeleton::GetEndTime(std::string_view _str)
{
	std::string str = HString::Upper(_str.data());
	return AnimationTime[str].second;
}
