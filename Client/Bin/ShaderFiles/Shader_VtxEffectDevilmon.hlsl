#include "Engine_Shader_Defines.hlsli"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
vector g_Scale;
matrix g_vCamPosition;
texture2D g_Texture;
texture2D g_DepthTexture;
texture2D g_Dissolve;
texture2D g_Mask;

float Time;
int Count;
float Frame;

struct VS_IN_EFFECT
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT_EFFECT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
};

VS_OUT_EFFECT VS_MAIN_EFFECT(VS_IN_EFFECT In)
{
    VS_OUT_EFFECT Out;
    
    matrix matWV, matWVP;
    
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    float2 Offset = In.vTexcoord - 0.5f;

    float4 CamPosition = g_vCamPosition[3];
    
    float4 WorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    
    float3 vLook = normalize(CamPosition - WorldPos);
    float3 rightDir = normalize(cross(float3(0.f, 1.f, 0.f), vLook));
    float3 upDir = normalize(cross(vLook, rightDir));

    float3 dirRight = rightDir * g_Scale.x * Offset.x;
    float3 dirUp = upDir * g_Scale.y * Offset.y;
    
    float3 worldPos = In.vPosition + dirRight + dirUp;

    Out.vPosition = mul(float4(worldPos, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;
    Out.vProjPos = Out.vPosition;

    return Out;
}

struct PS_IN_EFFECT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT_EFFECT
{
    float4 vColor : SV_TARGET0;
};

PS_OUT_EFFECT PS_MAIN_EFFECT(PS_IN_EFFECT In)
{
    PS_OUT_EFFECT Out;
    
    int totalFrame = Count * Count;
    
    int currentFrame = int(Time / Frame) % totalFrame;

    int frameX = currentFrame % Count;
    int frameY = currentFrame / Count;

    float frameWidth = 1.0 / Count;
    float frameHeight = 1.0 / Count;
    
    
    float2 uv = In.vTexcoord;
    uv.y = 1.f - uv.y;
    uv.x = uv.x * frameWidth + frameX * frameWidth;
    uv.y = uv.y * frameHeight + frameY * frameHeight;
    
    float4 vcolor = g_Texture.Sample(DefaultSampler, uv);
    
    Out.vColor = vcolor;
    
    return Out;
}

struct VS_IN_SOFTEFFECT
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT_SOFTEFFECT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
};

VS_OUT_SOFTEFFECT VS_MAIN_SOFTEFFECT(VS_IN_SOFTEFFECT In)
{
    VS_OUT_SOFTEFFECT Out;
    
    matrix matWV, matWVP;
    
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    float2 Offset = In.vTexcoord - 0.5f; // (-0.5 ~ 0.5)

    float4 CamPosition = g_vCamPosition[3];
    
    float4 WorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    
    float3 vLook = normalize(CamPosition - WorldPos);
    float3 rightDir = normalize(cross(float3(0.f, 1.f, 0.f), vLook));
    float3 upDir = normalize(cross(vLook, rightDir));

    float3 dirRight = rightDir * g_Scale.x * Offset.x;
    float3 dirUp = upDir * g_Scale.y * Offset.y;
    
    float3 worldPos = In.vPosition + dirRight + dirUp;

    Out.vPosition = mul(float4(worldPos, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;
    Out.vProjPos = Out.vPosition;

    return Out;
}

struct PS_IN_SOFTEFFECT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
};

struct PS_OUT_SOFTEFFECT
{
    float4 vColor : SV_TARGET0;
};


PS_OUT_SOFTEFFECT PS_MAIN_SOFTEFFECT(PS_IN_SOFTEFFECT In)
{
    PS_OUT_SOFTEFFECT Out;
    
    float4 vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
    float2 vTexcoord;
    
    vTexcoord.x = In.vProjPos.x / In.vProjPos.w * 0.5f + 0.5f;
    vTexcoord.y = In.vProjPos.y / In.vProjPos.w * -0.5f + 0.5f;
    
    float4 vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vTexcoord);
    
    float fOldViewZ = vDepthDesc.y * 500.f;
    
    float fDistance = fOldViewZ - In.vProjPos.w;
    
    float4 Color = float4(250 / 255.0f, 142 / 255.0f, 229 / 255.0f, vColor.a);
    
    vColor = vColor * Color;
    
    Out.vColor = vColor;
    
    return Out;
}

technique11 DefaultTechnique
{
    pass DevilmonSkill3Hnad
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN_EFFECT();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_EFFECT();
    }

    pass DevilmonSkill3Softeffect
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN_SOFTEFFECT();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SOFTEFFECT();
    }
}