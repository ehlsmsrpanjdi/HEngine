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
    float4x4 g_mWorldViewProjection;
};

cbuffer BoneMatrixBuffer : register(b1)
{
    matrix boneMatrices[100]; // �ִ� �� ���� �°� ����
}

cbuffer MeshMatrix : register(b2)
{
    float4x4 MeshGlobalMatrix; // �޽��� �θ���ġ �������
}

cbuffer DirectionalLightBuffer : register(b3)
{
    float3 LightDirection; // 12 bytes
    float Padding1; // 4 bytes (�е��� �߰��Ͽ� 16����Ʈ ����)
    float3 LightColor; // 12 bytes
    float LightIntensity; // 4 bytes
};

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
        if (boneIndex == -1)
        {
            input.position = mul(MeshGlobalMatrix, input.position);
    
            output.position = mul(g_mWorldViewProjection, input.position);
            output.Textcoord = input.Textcoord;
            return output;
        }
        // ��ȿ�� ����ġ�� �ݿ�
        if (weight > 0.0001f)
        {
            float4 transformed = mul(boneMatrices[boneIndex], input.position);
            skinnedPos += transformed * weight;
        }
    }

  
    output.position = mul(g_mWorldViewProjection, skinnedPos);
    output.Textcoord = input.Textcoord;
    
    //�ؿ����� �����
    output.normal = mul((float3x3) g_mWorldViewProjection, input.normal);

    return output;
}

float4 psmain(PS_INPUT input) : SV_Target
{
    
    float4 color = texture0.Sample(sampler0, input.Textcoord);
    
    float ka = 1.f; // Ambient light intensity
    
    float3 ia = float3(1.0, 1.0, 1.0);
    
    float3 ambiend_light = ka * ia;
    
    float kd = 1.0f;
    float3 id = float3(1.0, 1.0, 1.0);
    float amount_diffuse_light = max(0.0, dot(LightDirection, input.normal));
    
    float3 diffuse_light = kd * amount_diffuse_light * id;
    
    float3 final_light = ambiend_light + diffuse_light;
    
    float3 final_color = color.rgb * final_light;
    
    return float4(final_color, 1.0f);
    
    
    //float4 color = texture0.Sample(sampler0, input.Textcoord);
    //return color;
}
