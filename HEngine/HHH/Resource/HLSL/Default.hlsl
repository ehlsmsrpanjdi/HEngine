struct VS_INPUT
{
    float4 position : POSITION;
    float2 Textcoord : TEXCOORD;
    float2 Padding : PADDING;
    float4 normal : NORMAL;
    uint boneIndices[4] : BONEINDICES;
    float boneWeights[4] : BONEWEIGHTS;
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

cbuffer BoneMatrixBuffer : register(b1)
{
    matrix boneMatrices[100]; // 최대 본 수에 맞게 설정
}

cbuffer MeshMatrix : register(b2)
{
    float4x4 MeshGlobalMatrix; // 메쉬들 부모위치 적용행렬
}

cbuffer DirectionalLightBuffer : register(b3)
{
    float3 LightDirection; // 12 bytes
    float Padding1; // 4 bytes (패딩을 추가하여 16바이트 정렬)
    float3 LightColor; // 12 bytes
    float LightIntensity; // 4 bytes
};

VS_OUTPUT vsmain(VS_INPUT input)
{
    VS_OUTPUT output;
    
    // 스키닝된 위치 초기화
    float4 skinnedPos = float4(0, 0, 0, 0);

    // 본 인덱스/가중치를 사용한 스키닝
    for (int i = 0; i < 4; ++i)
    {
        uint boneIndex = input.boneIndices[i];
        float weight = input.boneWeights[i];
        // 유효한 가중치만 반영
        if (weight > 0.0001f)
        {
            float4 transformed = mul(boneMatrices[boneIndex], input.position);
            skinnedPos += transformed * weight;
        }
    }
    
    output.position = mul(WVPMatrix, skinnedPos);
    
    output.Textcoord = input.Textcoord;
    
    //밑에꺼가 생긴거
    output.normal = mul((float3x3) WVPMatrix, input.normal);

    return output;
}

float4 psmain(PS_INPUT input) : SV_Target
{
    float4 color = texture0.Sample(sampler0, input.Textcoord);
    
    float3 normal = normalize(input.normal);
    float3 lightDir = normalize(-LightDirection.xyz);

    float ka = 0.1f; //밝기
    float3 ia = float3(1.0, 1.0, 1.0);
    float3 ambient_light = ka * ia;

    float kd = 1.0f;
    float3 id = float3(1.0, 1.0, 1.0);
    float amount_diffuse_light = max(0.0, dot(lightDir, normal));
    float3 diffuse_light = kd * amount_diffuse_light * id;

    float3 final_light = saturate(ambient_light + diffuse_light);

    float3 final_color = color.rgb * final_light;
    
    return float4(final_color, 1.0f);
    
    //범프맵핑
    
    
    //float ka = 1.f; // Ambient light intensity
    
    //float3 ia = float3(1.0, 1.0, 1.0);
    
    //float3 ambiend_light = ka * ia;
    
    //float kd = 1.0f;
    //float3 id = float3(1.0, 1.0, 1.0);
    //float amount_diffuse_light = max(0.0, dot(LightDirection, input.normal));
    
    //float3 diffuse_light = kd * amount_diffuse_light * id;
    
    //float3 final_light = ambiend_light + diffuse_light;
    
    //float3 final_color = color.rgb * final_light;
    
    //return float4(final_color, 1.0f);
    
    //float4 color = texture0.Sample(sampler0, input.Textcoord);
    //return color;
}
