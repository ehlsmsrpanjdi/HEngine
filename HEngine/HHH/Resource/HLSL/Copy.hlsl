struct VS_INPUT
{
    float4 position : POSITION;
    float2 texcoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};

cbuffer WVP : register(b0)
{
    float4x4 WVPMatrix;
}



VS_OUTPUT vsmain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.position = mul(WVPMatrix, input.position);

    output.texcoord = input.texcoord;
    return output;
}

Texture2D Texture : register(t0);
sampler TextureSampler : register(s0);



struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};


float4 psmain(PS_INPUT input) : SV_TARGET
{
    return Texture.Sample(TextureSampler, input.texcoord);
}