struct VS_INPUT
{
    float4 position : POSITION;
    float2 Textcoord : TEXCOORD;
    float2 Padding : PADDING;
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
    float4x4 WVPMatrix;
};

cbuffer MeshMatrix : register(b2)
{
    float4x4 MeshGlobalMatrix; // 메쉬들 부모위치 적용행렬
}

VS_OUTPUT vsmain(VS_INPUT input)
{
    VS_OUTPUT output;
   
    input.position = mul(MeshGlobalMatrix, input.position);
    
    output.position = mul(WVPMatrix, input.position);
    output.Textcoord = input.Textcoord;
    return output;
}

float4 psmain(PS_INPUT input) : SV_Target
{
    float4 color = texture0.Sample(sampler0, input.Textcoord);
    return color;
}
