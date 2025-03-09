cbuffer constant : register(b0)
{
    float4x4 WorldViewProj;
}


float4 vsmain(float4 pos : POSITION) : SV_POSITION
{
    pos = mul(pos, WorldViewProj);
    return pos;
}

float4 psmain(float4 pos : SV_POSITION) : SV_Target
{
    return float4(1.f, 1.f, 1.f, 1.0f);
} 