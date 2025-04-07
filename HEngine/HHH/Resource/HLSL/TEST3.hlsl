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


cbuffer WVP : register(b0)
{
    float4x4 WVPMatrix;
}

float4 vsmain(VS_INPUT input) : SV_Position
{
    VS_OUTPUT output;
    output.position = mul(WVPMatrix, input.position);
    
    return output.position;
}

float4 psmain(float4 input : SV_Position ) : SV_Target
{
    return float4(1.0f, 1.0f, 1.0f, 0.5f);
}