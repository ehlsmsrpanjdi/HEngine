#include "EngineTransform.h"

EngineTransform::~EngineTransform()
{
    /*scaleA = XMFLOAT3(scaleA.x, scaleA.y, scaleA.z);
    scaleB = XMFLOAT3(scaleB.x, scaleB.y, scaleB.z);*/
}

bool EngineTransform::OBB(const EngineTransform& other) const {
    // ���� ��ǥ��� ��ȯ�� �߽� ��ǥ
    XMVECTOR posA = XMLoadFloat3(&this->position);
    XMVECTOR posB = XMLoadFloat3(&other.position);

    XMVECTOR quatA = XMLoadFloat4(&this->rotation);
    XMVECTOR quatB = XMLoadFloat4(&other.rotation);

    XMMATRIX rotA = XMMatrixRotationQuaternion(quatA);
    XMMATRIX rotB = XMMatrixRotationQuaternion(quatB);

    // ������ �ݿ�
    XMFLOAT3 scaleA = this->scale;
    XMFLOAT3 scaleB = other.scale;

    // OBB�� �ݻ����� ���
    float halfA[3] = { scaleA.x * 0.5f, scaleA.y * 0.5f, scaleA.z * 0.5f };
    float halfB[3] = { scaleB.x * 0.5f, scaleB.y * 0.5f, scaleB.z * 0.5f };

    // �� OBB�� ���� �� (���� ����)
    XMVECTOR axisA[3] = {
        XMVector3Normalize(rotA.r[0]),  // X��
        XMVector3Normalize(rotA.r[1]),  // Y��
        XMVector3Normalize(rotA.r[2])   // Z��
    };

    XMVECTOR axisB[3] = {
        XMVector3Normalize(rotB.r[0]),
        XMVector3Normalize(rotB.r[1]),
        XMVector3Normalize(rotB.r[2])
    };

    // �߽� �� ����
    XMVECTOR D = XMVectorSubtract(posB, posA);

    // SAT: �� 15���� �࿡ ���� ���� �˻�
    for (int i = 0; i < 3; ++i) {
        XMVECTOR axis = axisA[i];
        if (!IsOverLapOnAxis(axis, D, axisA, halfA, axisB, halfB))
            return false;
    }

    for (int i = 0; i < 3; ++i) {
        XMVECTOR axis = axisB[i];
        if (!IsOverLapOnAxis(axis, D, axisA, halfA, axisB, halfB))
            return false;
    }

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            XMVECTOR axis = XMVector3Cross(axisA[i], axisB[j]);
            // ���� ���� ���̰� 1e-5f ������ �� �������� �ʵ��� ���� ����
            if (XMVector3LengthSq(axis).m128_f32[0] < 1e-8f) continue; // �ʹ� ���� ���� ����
            if (!IsOverLapOnAxis(axis, D, axisA, halfA, axisB, halfB))
                return false;
        }
    }

    return true; // 15�� �� ���� ��ħ �� �浹
}


bool EngineTransform::SphereCollision(const EngineTransform& other) const {
    // A ���� �߽� (position)�� B ���� �߽� (other.position)
    XMVECTOR posA = XMLoadFloat3(&this->position);
    XMVECTOR posB = XMLoadFloat3(&other.position);

    // �� �߽� �� �Ÿ� ���
    XMVECTOR distanceVec = XMVectorSubtract(posB, posA);
    float distance = XMVectorGetX(XMVector3Length(distanceVec));

    // ������ ��� (���⼭�� scale.x�� ���������� ����)
    float radiusA = this->scale.x * 0.5f; // scale.x�� ���������� ���
    float radiusB = other.scale.x * 0.5f;

    // �߽� �� �Ÿ��� �� ���� ������ �պ��� ������ �浹
    return distance < (radiusA + radiusB);
}