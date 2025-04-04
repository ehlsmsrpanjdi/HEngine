struct VS_INPUT
{
    float4 position : POSITION;
    float2 Textcoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 Textcoord : TEXCOORD;
};

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 Textcoord : TEXCOORD;
};

SamplerState sampler0 : register(s0);
Texture2D texture0 : register(t0);

cbuffer WVP : register(b0)
{
    float4x4 g_mWorldViewProjection;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = mul(input.position, g_mWorldViewProjection);
    output.Textcoord = input.Textcoord;
    return output;
}

float4 psmain(PS_INPUT input) : SV_Target
{
    float4 color = texture0.Sample(sampler0, input.Textcoord);
    return color;
}