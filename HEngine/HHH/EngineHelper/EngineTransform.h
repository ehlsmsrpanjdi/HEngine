#pragma once
#include <vector>
#include "DirectXMath.h"

using namespace DirectX;

class EngineTransform {
public:
	EngineTransform() : position(0.0f, 0.0f, 0.0f),
		rotation(0.0f, 0.0f, 0.0f, 1.0f),
		scale(1.0f, 1.0f, 1.0f)
	{
	}
	~EngineTransform();

	// 위치 (Translation)
	XMFLOAT3 position;
	// 회전 (Rotation)
	XMFLOAT4 rotation;
	// 크기 (Scale)
	XMFLOAT3 scale;

	XMMATRIX Identity() {
		return XMMatrixIdentity();
	}

	void AddLocation(const XMFLOAT3& _position) {
		position.x += _position.x;
		position.y += _position.y;
		position.z += _position.z;
	}

	void AddLocation(float _x, float _y = 0.f, float _z = 0.f) {
		position.x += _x;
		position.y += _y;
		position.z += _z;
	}

	void SetLocation(float _x, float _y, float _z) {
		position.x = _x;
		position.y = _y;
		position.z = _z;
	}

	void AddRotation(const XMFLOAT4& _Rotation) {
		XMVECTOR currentRotation = XMLoadFloat4(&rotation);
		XMVECTOR additionalRotation = XMLoadFloat4(&_Rotation);
		XMVECTOR newRotation = XMQuaternionMultiply(currentRotation, additionalRotation);
		XMStoreFloat4(&rotation, newRotation);
	}

	void AddRotation(float _x, float _y = 0, float _z = 0) {
		// 회전 각도를 쿼터니언으로 변환
		XMVECTOR additionalRotationX = XMQuaternionRotationRollPitchYaw(_x, 0.0f, 0.0f);
		XMVECTOR additionalRotationY = XMQuaternionRotationRollPitchYaw(0.0f, _y, 0.0f);
		XMVECTOR additionalRotationZ = XMQuaternionRotationRollPitchYaw(0.0f, 0.0f, _z);

		// 현재 회전을 로드
		XMVECTOR currentRotation = XMLoadFloat4(&rotation);

		// 쿼터니언 곱셈을 사용하여 회전을 누적
		XMVECTOR newRotation = XMQuaternionMultiply(currentRotation, additionalRotationX);
		newRotation = XMQuaternionMultiply(newRotation, additionalRotationY);
		newRotation = XMQuaternionMultiply(newRotation, additionalRotationZ);

		// 결과를 저장
		XMStoreFloat4(&rotation, newRotation);
	}

	void AddScale(const XMFLOAT3& _Scale) {
		scale.x += _Scale.x;
		scale.y += _Scale.y;
		scale.z += _Scale.z;
	}

	void AddScale(float _x, float _y = 0.f, float _z = 0.f) {
		scale.x += _x;
		scale.y += _y;
		scale.z += _z;
	}


	void SetLocation(const XMFLOAT3& _position) {
		position = _position;
	}

	void SetRotation(const XMFLOAT4& _Rotation) {
		rotation = _Rotation;
	}

	void SetScale(const XMFLOAT3& _Scale) {
		scale = _Scale;
	}

	// 각 변환 행렬을 개별적으로 계산하고, 최종 월드 행렬을 반환
	XMMATRIX GetWorldMatrix() const
	{
		// 1. Translation 행렬 계산
		XMMATRIX translationMat = XMMatrixTranslation(position.x, position.y, position.z);

		// 2. Rotation 행렬 계산 (Quaternion을 사용)
		XMVECTOR quaternion = XMLoadFloat4(&rotation);
		XMMATRIX rotationMat = XMMatrixRotationQuaternion(quaternion);

		// 3. Scale 행렬 계산
		XMMATRIX scaleMat = XMMatrixScaling(scale.x, scale.y, scale.z);

		// Scale * Rotation * Translation 순서로 곱하기
		return scaleMat * rotationMat * translationMat; // S * R * T
	}
};

