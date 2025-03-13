//#pragma once
//#include <memory>
//#include "Vector3D.h"
//class EngineTransform
//{
//public:
//	EngineTransform()
//	{
//	}
//
//	void setIdentity()
//	{
//		::memset(m_mat, 0, sizeof(float) * 16);
//		m_mat[0][0] = 1;
//		m_mat[1][1] = 1;
//		m_mat[2][2] = 1;
//		m_mat[3][3] = 1;
//	}
//
//	void setLocation(const Vector3D& translation)
//	{
//		setIdentity();
//		m_mat[3][0] = translation.m_x;
//		m_mat[3][1] = translation.m_y;
//		m_mat[3][2] = translation.m_z;
//	}
//
//	void setScale(const Vector3D& scale)
//	{
//		setIdentity();
//		m_mat[0][0] = scale.m_x;
//		m_mat[1][1] = scale.m_y;
//		m_mat[2][2] = scale.m_z;
//	}
//
//	void setRotationX(float x)
//	{
//		m_mat[1][1] = cos(x);
//		m_mat[1][2] = sin(x);
//		m_mat[2][1] = -sin(x);
//		m_mat[2][2] = cos(x);
//	}
//
//	void setRotationY(float y)
//	{
//		m_mat[0][0] = cos(y);
//		m_mat[0][2] = -sin(y);
//		m_mat[2][0] = sin(y);
//		m_mat[2][2] = cos(y);
//	}
//
//	void setRotationZ(float z)
//	{
//		m_mat[0][0] = cos(z);
//		m_mat[0][1] = sin(z);
//		m_mat[1][0] = -sin(z);
//		m_mat[1][1] = cos(z);
//	}
//
//	void addLocation
//
//
//	void operator *=(const EngineTransform& matrix)
//	{
//		EngineTransform out;
//		for (int i = 0; i < 4; i++)
//		{
//			for (int j = 0; j < 4; j++)
//			{
//				out.m_mat[i][j] =
//					Transform[i][0] * matrix.m_mat[0][j] + m_mat[i][1] * matrix.m_mat[1][j] +
//					Transform[i][2] * matrix.m_mat[2][j] + m_mat[i][3] * matrix.m_mat[3][j];
//			}
//		}
//		setMatrix(out);
//	}
//
//	void setMatrix(const EngineTransform& matrix)
//	{
//		::memcpy(m_mat, matrix.m_mat, sizeof(float) * 16);
//	}
//
//	void setOrthoLH(float width, float height, float near_plane, float far_plane)
//	{
//		setIdentity();
//		m_mat[0][0] = 2.0f / width;
//		m_mat[1][1] = 2.0f / height;
//		m_mat[2][2] = 1.0f / (far_plane - near_plane);
//		m_mat[3][2] = -(near_plane / (far_plane - near_plane));
//	}
//
//	~EngineTransform()
//	{
//	}
//
//public:
//	float Transform[4][4] = {};
//};


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

