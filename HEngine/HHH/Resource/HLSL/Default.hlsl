struct VS_INPUT
{
    float4 position : POSITION;
    float2 Textcoord : TEXCOORD;
    uint boneIndices[4] : BONEINDICES;
    float boneWeights[4] : BONEWEIGHTS;
    float3 normal : NORMAL;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 Textcoord : TEXCOORD;
    float3 normal : NORMAL;
};

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 Textcoord : TEXCOORD;
    float3 normal : NORMAL;
};

SamplerState sampler0 : register(s0);
Texture2D texture0 : register(t0);

cbuffer WVP : register(b0)
{
    float4x4 g_mWorldViewProjection;
};

cbuffer BoneMatrixBuffer : register(b1)
{
    matrix boneMatrices[100]; // 최대 본 수에 맞게 설정
}

cbuffer MeshMatrix : register(b2)
{
    float4x4 MeshGlobalMatrix; // 메쉬들 부모위치 적용행렬
}

cbuffer DirectionalLightBuffer : register(b3)
{
    float3 LightDirection; // 12 bytes
    float Padding1; // 4 bytes (패딩을 추가하여 16바이트 정렬)
    float3 LightColor; // 12 bytes
    float LightIntensity; // 4 bytes
};

VS_OUTPUT vsmain(VS_INPUT input)
{
    VS_OUTPUT output;

    // 스키닝된 위치 초기화
    float4 skinnedPos = float4(0, 0, 0, 0);

    // 본 인덱스/가중치를 사용한 스키닝
    for (int i = 0; i < 4; ++i)
    {
        uint boneIndex = input.boneIndices[i];
        float weight = input.boneWeights[i];
        if (boneIndex == -1)
        {
            input.position = mul(MeshGlobalMatrix, input.position);
    
            output.position = mul(g_mWorldViewProjection, input.position);
            output.Textcoord = input.Textcoord;
            return output;
        }
        // 유효한 가중치만 반영
        if (weight > 0.0001f)
        {
            float4 transformed = mul(boneMatrices[boneIndex], input.position);
            skinnedPos += transformed * weight;
        }
    }

  
    output.position = mul(g_mWorldViewProjection, skinnedPos);
    output.Textcoord = input.Textcoord;
    
    //밑에꺼가 생긴거
    
    float3x3 normalMatrix = (float3x3) MeshGlobalMatrix;
    output.normal = normalize(mul(normalMatrix, input.normal));

    return output;
}

float4 psmain(PS_INPUT input) : SV_Target
{
        // 조명 방향과 법선 벡터를 이용해 디퓨즈 값 계산
    float3 lightDir = normalize(LightDirection); // 라이트 방향 (단위 벡터)
    float3 norm = normalize(input.normal); // 표면 법선 벡터 (단위 벡터)

    // 디퓨즈 조명 계산
    float diffuse = max(dot(norm, lightDir), 0.0f);

    // 텍스처 샘플링
    float4 textureColor = texture0.Sample(sampler0, input.Textcoord);

    // 조명 적용: 색상에 디퓨즈 조명 값을 곱하기
    float4 color = textureColor * diffuse * float4(LightColor, 1.0f) * LightIntensity;

    color = float4(diffuse.xxx, 1.0f);

    return color;
    
    //float4 color = texture0.Sample(sampler0, input.Textcoord);
    //return color;
}