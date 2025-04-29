#include "EngineTransform.h"

EngineTransform::~EngineTransform()
{
    /*scaleA = XMFLOAT3(scaleA.x, scaleA.y, scaleA.z);
    scaleB = XMFLOAT3(scaleB.x, scaleB.y, scaleB.z);*/
}

bool EngineTransform::OBB(const EngineTransform& A, const EngineTransform& B)
{
    // 월드 좌표계로 변환된 중심 좌표
    XMVECTOR posA = XMLoadFloat3(&A.position);
    XMVECTOR posB = XMLoadFloat3(&B.position);

    XMVECTOR quatA = XMLoadFloat4(&A.rotation);
    XMVECTOR quatB = XMLoadFloat4(&B.rotation);

    XMMATRIX rotA = XMMatrixRotationQuaternion(quatA);
    XMMATRIX rotB = XMMatrixRotationQuaternion(quatB);


    // 스케일 반영
    XMFLOAT3 scaleA = A.scale;
    XMFLOAT3 scaleB = B.scale;

    float Value = 200.0f;

    scaleA = XMFLOAT3(scaleA.x * Value, scaleA.y * Value, scaleA.z * Value);
    scaleB = XMFLOAT3(scaleB.x * Value, scaleB.y * Value, scaleB.z * Value);

    // OBB의 반사이즈 계산
    float halfA[3] = { scaleA.x * 0.5f, scaleA.y * 0.5f, scaleA.z * 0.5f };
    float halfB[3] = { scaleB.x * 0.5f, scaleB.y * 0.5f, scaleB.z * 0.5f };

    // 각 OBB의 로컬 축 (단위 벡터)
    XMVECTOR axisA[3] = {
        XMVector3Normalize(rotA.r[0]),  // X축
        XMVector3Normalize(rotA.r[1]),  // Y축
        XMVector3Normalize(rotA.r[2])   // Z축
    };

    XMVECTOR axisB[3] = {
        XMVector3Normalize(rotB.r[0]),
        XMVector3Normalize(rotB.r[1]),
        XMVector3Normalize(rotB.r[2])
    };

    // 중심 간 벡터
    XMVECTOR D = XMVectorSubtract(posB, posA);

    // SAT: 총 15개의 축에 대해 투영 검사
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
            // 교차 벡터 길이가 1e-5f 이하일 때 무시하지 않도록 범위 조정
            if (XMVector3LengthSq(axis).m128_f32[0] < 1e-8f) continue; // 너무 작은 값은 무시
            if (!IsOverLapOnAxis(axis, D, axisA, halfA, axisB, halfB))
                return false;
        }
    }

    return true; // 15개 축 전부 겹침 → 충돌
}

bool EngineTransform::SphereCollision(const EngineTransform& A, const EngineTransform& B)
{
    // A 원의 중심 (position)과 B 원의 중심 (other.position)
    XMVECTOR posA = XMLoadFloat3(&A.position);
    XMVECTOR posB = XMLoadFloat3(&B.position);

    // 두 중심 간 거리 계산
    XMVECTOR distanceVec = XMVectorSubtract(posB, posA);
    float distance = XMVectorGetX(XMVector3Length(distanceVec));

    // 반지름 계산 (여기서는 scale.x가 반지름으로 가정)
    float radiusA = A.scale.x * 89.f; // scale.x를 반지름으로 사용
    float radiusB = B.scale.x * 89.f;

    // 중심 간 거리가 두 원의 반지름 합보다 작으면 충돌
    return distance < (radiusA + radiusB);
}
