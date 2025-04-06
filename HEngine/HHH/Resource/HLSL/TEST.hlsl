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

cbuffer WVP : register(b0)
{
    float4x4 WVPMatrix;
}

Texture2D Texture0 : register(t0);
SamplerState Sampler0 : register(s0);

VS_OUTPUT vsmain(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = mul(WVPMatrix, input.position);
    output.Textcoord = input.Textcoord;
    
    return output;
}

float4 psmain(PS_INPUT input) : SV_Target
{
    float4 color = Texture0.Sample(Sampler0, input.Textcoord);
    return color;
}