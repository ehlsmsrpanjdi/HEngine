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

	// ��ġ (Translation)
	XMFLOAT3 position;
	// ȸ�� (Rotation)
	XMFLOAT4 rotation;
	// ũ�� (Scale)
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
		// ȸ�� ������ ���ʹϾ����� ��ȯ
		XMVECTOR additionalRotationX = XMQuaternionRotationRollPitchYaw(_x, 0.0f, 0.0f);
		XMVECTOR additionalRotationY = XMQuaternionRotationRollPitchYaw(0.0f, _y, 0.0f);
		XMVECTOR additionalRotationZ = XMQuaternionRotationRollPitchYaw(0.0f, 0.0f, _z);

		// ���� ȸ���� �ε�
		XMVECTOR currentRotation = XMLoadFloat4(&rotation);

		// ���ʹϾ� ������ ����Ͽ� ȸ���� ����
		XMVECTOR newRotation = XMQuaternionMultiply(currentRotation, additionalRotationX);
		newRotation = XMQuaternionMultiply(newRotation, additionalRotationY);
		newRotation = XMQuaternionMultiply(newRotation, additionalRotationZ);

		// ����� ����
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

	// �� ��ȯ ����� ���������� ����ϰ�, ���� ���� ����� ��ȯ
	XMMATRIX GetWorldMatrix() const
	{
		// 1. Translation ��� ���
		XMMATRIX translationMat = XMMatrixTranslation(position.x, position.y, position.z);

		// 2. Rotation ��� ��� (Quaternion�� ���)
		XMVECTOR quaternion = XMLoadFloat4(&rotation);
		XMMATRIX rotationMat = XMMatrixRotationQuaternion(quaternion);

		// 3. Scale ��� ���
		XMMATRIX scaleMat = XMMatrixScaling(scale.x, scale.y, scale.z);

		// Scale * Rotation * Translation ������ ���ϱ�
		return scaleMat * rotationMat * translationMat; // S * R * T
	}

	XMMATRIX GetInverseMatrix() {
		XMVECTOR determinant;
		XMMATRIX inverseMatrix = DirectX::XMMatrixInverse(&determinant, GetWorldMatrix());

		// ��Ľ��� 0�� ���
		if (XMVectorGetX(determinant) == 0.0f) {
			return XMMATRIX();
		}
		else {
			return inverseMatrix;
		}
	}

	XMMATRIX GetViewMatrix() const {
		// ī�޶��� ��ġ
		XMVECTOR eyePosition = XMLoadFloat3(&position);

		// ī�޶��� ȸ�� (���ʹϾ�)
		XMVECTOR quaternion = XMLoadFloat4(&rotation);
        XMMATRIX rotationMatrix = DirectX::XMMatrixRotationQuaternion(quaternion);

		// ī�޶��� ���� ���� (�⺻������ -Z ����)
		XMVECTOR forward = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
		forward = XMVector3Transform(forward, rotationMatrix);

		// ī�޶��� �� ���� (�⺻������ Y ����)
		XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		up = XMVector3Transform(up, rotationMatrix);

		// �� ��� ���
		return DirectX::XMMatrixLookToLH(eyePosition, forward, up);
	}

	XMMATRIX GetRotationMatrix(const XMFLOAT4& rotation) {
		return XMMatrixRotationQuaternion(XMLoadFloat4(&rotation));
	}

	void Move(float x, float y, float z) {
		// 1. ȸ�� ��� ��������
		XMMATRIX rotationMatrix = GetRotationMatrix(rotation);

		// 2. �Է� �̵� ���� ���� (���� ����)
		XMVECTOR localMovement = XMVectorSet(x, y, z, 0);

		// 3. ȸ�� ���� (���� �� ���� ��ȯ)
		XMVECTOR worldMovement = XMVector3TransformNormal(localMovement, rotationMatrix);

		// 4. ��ȯ�� �̵� ���͸� ��ġ�� ����
		XMFLOAT3 movement;
		XMStoreFloat3(&movement, worldMovement);
		position.x += movement.x;
		position.y += movement.y;
		position.z += movement.z;
	}

	void Rotate(float _x, float _y = 0, float _z = 0) {
		// X, Y, Z ������ ���� �������� ȸ�� ���ʹϾ� ����
		XMVECTOR additionalRotationX = XMQuaternionRotationAxis(XMVectorSet(1, 0, 0, 0), _y); // X�� (Pitch)
		XMVECTOR additionalRotationY = XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 0), _x); // Y�� (Yaw)
		XMVECTOR additionalRotationZ = XMQuaternionRotationAxis(XMVectorSet(0, 0, 1, 0), _z); // Z�� (Roll)

		// ���� ȸ���� �ε�
		XMVECTOR currentRotation = XMLoadFloat4(&rotation);

		// ���� ȸ�� ���� (�߰� ȸ�� * ���� ȸ�� ����)
		XMVECTOR newRotation = XMQuaternionMultiply(additionalRotationZ, currentRotation); // Roll
		newRotation = XMQuaternionMultiply(additionalRotationX, newRotation); // Pitch
		newRotation = XMQuaternionMultiply(additionalRotationY, newRotation); // Yaw

		// ��� ����
		XMStoreFloat4(&rotation, newRotation);
	}
};

