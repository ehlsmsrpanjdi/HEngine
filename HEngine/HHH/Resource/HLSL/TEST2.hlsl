struct VS_INPUT
{
    float4 position : POSITION;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
};

struct PS_INPUT
{
    float4 position : SV_POSITION;
};

cbuffer WVP : register(b0)
{
    float4x4 WVPMatrix;
}

VS_OUTPUT vsmain(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = mul(WVPMatrix, input.position);
    return output;
}

float4 psmain(PS_INPUT input) : SV_Target
{
    return float4(1.0f, 1.0f, 1.0f, 0.5f);
}