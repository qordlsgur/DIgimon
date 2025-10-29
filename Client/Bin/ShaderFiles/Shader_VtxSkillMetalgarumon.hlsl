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

struct VS_IN_MISSILE
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float3 vTangent : TANGENT;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT_MISSILE
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

VS_OUT_MISSILE VS_MISSILE(VS_IN_MISSILE In)
{
    VS_OUT_MISSILE Out;
    
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

struct PS_IN_MISSILE
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

struct PS_OUT_MISSILE
{
    float4 vDiffuse : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
};

PS_OUT_MISSILE PS_MISSILE(PS_IN_MISSILE In)
{
    PS_OUT_MISSILE Out;
    
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    
    if (vMtrlDiffuse.g <= 0.f)
        discard;
    
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = float4(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.0f, 0.0f, 0.0f);
    return Out;
}
    
struct VS_IN_LASER
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float3 vTangent : TANGENT;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT_LASER
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

VS_OUT_LASER VS_LASER(VS_IN_LASER In)
{
    VS_OUT_LASER Out;
    
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

struct PS_IN_LASER
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

struct PS_OUT_LASER
{
    float4 vDiffuse : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
};

PS_OUT_LASER PS_LASER(PS_IN_LASER In)
{
    PS_OUT_LASER Out;
    
    float u = In.vTexcoord.x;
    float v = In.vTexcoord.y;
    
    u = u + Time;
    v = v + Time;
    float2 uv = float2(u, v);
    
    float4 vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, uv);
    float4 vColor = g_Mask.Sample(DefaultSampler, In.vTexcoord);

    
    if (vColor.a <= 0.f)
        discard;
    
    vMtrlDiffuse = vMtrlDiffuse * vColor;
    
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = float4(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.0f, 0.0f, 0.0f);
    return Out;
}

PS_OUT_MISSILE PS_GMISSILE(PS_IN_MISSILE In)
{
    PS_OUT_MISSILE Out;
    

    float4 vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    float3 Color = float3(254, 253, 72) / 255.f;
    
    vMtrlDiffuse.rgb = vMtrlDiffuse.rgb * Color.rgb;
    
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = float4(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.0f, 0.0f, 0.0f);
    return Out;
}

PS_OUT_LASER PS_GLASER(PS_IN_LASER In)
{
    PS_OUT_LASER Out;
    
    
    float4 vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    float4 vColor = g_Mask.Sample(DefaultSampler, In.vTexcoord);

    
    if (vMtrlDiffuse.a <= 0.f)
        discard;
    
    vMtrlDiffuse = vMtrlDiffuse * vColor;
    
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = float4(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.0f, 0.0f, 0.0f);
    return Out;
}

technique11 DefaultTechnique
{
    pass Missile
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MISSILE();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MISSILE();
    } 

    pass Sseolleongpo
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_LASER();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_LASER();
    }

    pass GMissile
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MISSILE();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_GMISSILE();
    }

    pass GigaStorm
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_LASER();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_GLASER();
    }
}