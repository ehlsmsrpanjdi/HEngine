struct VS_INPUT
{
    float4 position : POSITION;
    float2 Textcoord : TEXCOORD;
    uint boneIndices[4] : BONEINDICES;
    float boneWeights[4] : BONEWEIGHTS;
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

cbuffer BoneMatrixBuffer : register(b1)
{
    matrix boneMatrices[100]; // �ִ� �� ���� �°� ����
}

cbuffer tempmatrix : register(b2)
{
    float4x4 globalmatrix; // �޽��� �θ���ġ �������
}

VS_OUTPUT vsmain(VS_INPUT input)
{
    VS_OUTPUT output;

    // ��Ű�׵� ��ġ �ʱ�ȭ
    float4 skinnedPos = float4(0, 0, 0, 0);

    // �� �ε���/����ġ�� ����� ��Ű��
    for (int i = 0; i < 4; ++i)
    {
        uint boneIndex = input.boneIndices[i];
        float weight = input.boneWeights[i];

        // ��ȿ�� ����ġ�� �ݿ�
        if (weight > 0.0001f)
        {
            float4 transformed = mul(boneMatrices[boneIndex], input.position);
            skinnedPos += transformed * weight;
        }
    }

    // ���� ��ġ�� WVP ��� ����
    output.position = mul(g_mWorldViewProjection, skinnedPos);
    output.Textcoord = input.Textcoord;
    return output;
    
    
    //VS_OUTPUT output;
    //input.position = mul(globalmatrix, input.position);
    
    //output.position = mul(g_mWorldViewProjection, input.position);
    //output.Textcoord = input.Textcoord;
    //return output;
}

float4 psmain(PS_INPUT input) : SV_Target
{
    float4 color = texture0.Sample(sampler0, input.Textcoord);
    return color;
}