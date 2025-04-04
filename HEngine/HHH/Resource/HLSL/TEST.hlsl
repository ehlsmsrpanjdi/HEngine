struct VS_INPUT
{
    float4 position : POSITION;
};

cbuffer WVP : register(b0)
{
    float4x4 WVPMatrix;
}


float4 vsmain(VS_INPUT input) : SV_POSITION
{
    float4 pos = mul(WVPMatrix, input.position);
    return pos;
}

float4 psmain(float4 pos : SV_POSITION) : SV_Target
{
    return float4(1.f, 1.f, 1.f, 1.0f);
} 