#include "Engine_Shader_Defines.hlsli"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_DiffuseTexture;
vector g_Scale;
matrix g_vCamPosition;

texture2D g_Texture;
texture2D g_DepthTexture;
texture2D g_Dissolve;
texture2D g_Mask;

float   Time;
int     Count;
float   Frame;

struct VS_IN_MODEL
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float3 vTangent : TANGENT;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT_MODEL
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

VS_OUT_MODEL VS_MAIN_MODEL(VS_IN_MODEL In)
{
    VS_OUT_MODEL Out;
  
    matrix matWV, matWVP;
    
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;
    Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
    Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    Out.vProjPos = Out.vPosition;
    return Out;
}

struct PS_IN_MODEL
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

struct PS_OUT_MODEL
{
    float4 vDiffuse : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
};

PS_OUT_MODEL PS_MAIN_MODEL(PS_IN_MODEL In)
{
    PS_OUT_MODEL Out;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    float3 Color = float3(37, 19, 39) / 255.0f;
    float4 Dissolve = g_Dissolve.Sample(DefaultSampler, In.vTexcoord);
        
    if (vMtrlDiffuse.r <= 0.25f)
        discard;
    
    vMtrlDiffuse.rgb = vMtrlDiffuse.rgb + Color;
    
    if (Dissolve.r <= Time)
        discard;
    
    vMtrlDiffuse.a = vMtrlDiffuse * Dissolve.a;
    
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = float4(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.0f, 0.0f, 0.0f);
    return Out;
}

struct VS_IN_EFFECT
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float3 vTangent : TANGENT;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT_EFFECT
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

VS_OUT_EFFECT VS_MAIN_EFFECT(VS_IN_EFFECT In)
{
    VS_OUT_EFFECT Out;
    
    matrix matWV, matWVP;
    
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;
    Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
    Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    Out.vProjPos = Out.vPosition;
    return Out;
}

struct PS_IN_EFFECT
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

struct PS_OUT_EFFECT
{
    float4 vDiffuse : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
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
    
    float4 vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, uv);

    float3 Color = float3(37, 19, 39) / 255.0f;
        
    vMtrlDiffuse.rgb = vMtrlDiffuse.rgb + Color;
    
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = float4(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.0f, 0.0f, 0.0f);
    return Out;
}

technique11 DefaultTechnique
{
    pass DevilmonSkill3
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN_MODEL();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_MODEL();
    }

    pass DevilmonSkill3Hnad
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN_EFFECT();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_EFFECT();
    }
}