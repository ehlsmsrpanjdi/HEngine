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
		return DirectX::XMMatrixIdentity();
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

	void SetScale(float _x, float _y, float _z) {
		scale = XMFLOAT3(_x, _y, _z);
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

	XMMATRIX GetInverseMatrix() {
		XMVECTOR determinant;
		XMMATRIX inverseMatrix = DirectX::XMMatrixInverse(&determinant, GetWorldMatrix());

		// 행렬식이 0인 경우
		if (XMVectorGetX(determinant) == 0.0f) {
			return XMMATRIX();
		}
		else {
			return inverseMatrix;
		}
	}

	XMMATRIX GetViewMatrix() const {
		// 카메라의 위치
		XMVECTOR eyePosition = XMLoadFloat3(&position);

		// 카메라의 회전 (쿼터니언)
		XMVECTOR quaternion = XMLoadFloat4(&rotation);
        XMMATRIX rotationMatrix = DirectX::XMMatrixRotationQuaternion(quaternion);

		// 카메라의 방향 벡터 (기본적으로 -Z 방향)
		XMVECTOR forward = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
		forward = XMVector3Transform(forward, rotationMatrix);

		// 카메라의 업 벡터 (기본적으로 Y 방향)
		XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		up = XMVector3Transform(up, rotationMatrix);

		// 뷰 행렬 계산
		return DirectX::XMMatrixLookToLH(eyePosition, forward, up);
	}

	XMMATRIX GetRotationMatrix(const XMFLOAT4& rotation) {
		return XMMatrixRotationQuaternion(XMLoadFloat4(&rotation));
	}

	void Move(float x, float y, float z) {
		// 1. 회전 행렬 가져오기
		XMMATRIX rotationMatrix = GetRotationMatrix(rotation);

		// 2. 입력 이동 벡터 생성 (로컬 기준)
		XMVECTOR localMovement = XMVectorSet(x, y, z, 0);

		// 3. 회전 적용 (로컬 → 월드 변환)
		XMVECTOR worldMovement = XMVector3TransformNormal(localMovement, rotationMatrix);

		// 4. 변환된 이동 벡터를 위치에 적용
		XMFLOAT3 movement;
		XMStoreFloat3(&movement, worldMovement);
		position.x += movement.x;
		position.y += movement.y;
		position.z += movement.z;
	}

	void Rotate(float _x, float _y = 0, float _z = 0) {
		// X, Y, Z 각각의 축을 기준으로 회전 쿼터니언 생성
		XMVECTOR additionalRotationX = XMQuaternionRotationAxis(XMVectorSet(1, 0, 0, 0), _y); // X축 (Pitch)
		XMVECTOR additionalRotationY = XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 0), _x); // Y축 (Yaw)
		XMVECTOR additionalRotationZ = XMQuaternionRotationAxis(XMVectorSet(0, 0, 1, 0), _z); // Z축 (Roll)

		// 현재 회전을 로드
		XMVECTOR currentRotation = XMLoadFloat4(&rotation);

		// 로컬 회전 적용 (추가 회전 * 기존 회전 순서)
		XMVECTOR newRotation = XMQuaternionMultiply(additionalRotationZ, currentRotation); // Roll
		newRotation = XMQuaternionMultiply(additionalRotationX, newRotation); // Pitch
		newRotation = XMQuaternionMultiply(additionalRotationY, newRotation); // Yaw

		// 결과 저장
		XMStoreFloat4(&rotation, newRotation);
	}
};

