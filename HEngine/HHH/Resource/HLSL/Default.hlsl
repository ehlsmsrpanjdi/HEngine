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
    float4x4 WorldMatrix;
    float4x4 ViewMatrix;
    float4x4 ProjectionMatrix;
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
    
    float4x4 g_mWorldViewProjection = ProjectionMatrix * ViewMatrix * WorldMatrix;

    output.position = mul(g_mWorldViewProjection, input.position);
    
    return output;
}

float4 psmain(PS_INPUT input) : SV_Target
{    
    //float4 color = texture0.Sample(sampler0, input.Textcoord);
    float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    return color;
}
