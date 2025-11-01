#include "Engine_Shader_Defines.hlsli"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
Texture2D g_DiffuseTexture;
Texture2D g_Texture;
Texture2D g_DepthTexture;
Texture2D g_Dissolve;
Texture2D g_Mask;

float Dir;
float Time;
float Radian;

/* 정점 쉐이더 : */
/* 정점에 대한 셰이딩 == 정점에 필요한 연산을 수행한다 == 정점의 상태변환(월드, 뷰, 투영) + 추가변환 */
/* 정점의 구성 정보를 수정, 변경한다 */ 
struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float3 vTangent : TANGENT;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;
  
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

/* 출력된 정점 위치벡터의 w값으로 모든 성분을 나눈다 -> 투영스페이스로 변환 */ 
/* 정점의 위치에 대해서 뷰포트 변환을 수행한다 */ 
/* 정점의 모든 정보를 보간하여 픽셀을 만든다. -> 래스터라이즈 */ 

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

struct PS_OUT
{
    float4 vDiffuse : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
};



/* 픽셀 쉐이더 : 픽셀의 최종적인 색을 결정하낟. */
PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;
    
    float2 uv = In.vTexcoord;
    //uv.x *= 4.f;
    uv.x -= Time;
    float3 Color = float3(1.f, 1.f, 1.f);
    
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, uv);
    
    if (vMtrlDiffuse.r <= 0.0f)
        discard;
    
    if (vMtrlDiffuse.r != 0.f)
        vMtrlDiffuse.rgb += Color;
    
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = float4(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.0f, 0.0f, 0.0f);
    return Out;
}

/* 픽셀 쉐이더 : 픽셀의 최종적인 색을 결정하낟. */
PS_OUT PS_Arrow(PS_IN In)
{
    PS_OUT Out;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    
    if (vMtrlDiffuse.r <= 0.4f)
        discard;
    
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = float4(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.0f, 0.0f, 0.0f);
    return Out;
}

/* 픽셀 쉐이더 : 픽셀의 최종적인 색을 결정하낟. */
PS_OUT PS_Cross(PS_IN In)
{
    PS_OUT Out;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    float3 Color = float3(250, 142, 229) / 255.0f;
    float4 Disslove = g_Dissolve.Sample(DefaultSampler, In.vTexcoord);
    
    if (vMtrlDiffuse.r <= 0.2f)
        vMtrlDiffuse.rgb = vMtrlDiffuse.rgb + Color;
    
    if (Time >= Disslove.r)
        discard;
    
    vMtrlDiffuse.a = vMtrlDiffuse.a * Disslove.a;
    
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = float4(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.0f, 0.0f, 0.0f);
    return Out;
}

struct VS_OUT_SKILL1EFFECT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
};

VS_OUT_SKILL1EFFECT VS_MAIN_SKILL1EFFECT(VS_IN In)
{
    VS_OUT_SKILL1EFFECT Out;
    
    matrix matWV, matWVP;
    
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    float2 Offset = In.vTexcoord - 0.5f; // (-0.5 ~ 0.5)

    float3 worldPos = In.vPosition;

    Out.vPosition = mul(float4(worldPos, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;
    Out.vProjPos = Out.vPosition;

    return Out;
}

struct PS_IN_SKILL1EFFECT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
};

struct PS_OUT_SKILL1EFFECT
{
    float4 vColor : SV_TARGET0;
};

PS_OUT_SKILL1EFFECT PS_MAIN_SKILL1EFFECT(PS_IN_SKILL1EFFECT In)
{
    PS_OUT_SKILL1EFFECT Out;

    float4 vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
    float4 Mask = g_Mask.Sample(DefaultSampler, In.vTexcoord);

    float2 vTexcoord;
    vTexcoord.x = In.vProjPos.x / In.vProjPos.w * 0.5f + 0.5f;
    vTexcoord.y = In.vProjPos.y / In.vProjPos.w * -0.5f + 0.5f;

    float4 vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vTexcoord);
    float fOldViewZ = vDepthDesc.y * 500.f;
    float fDistance = fOldViewZ - In.vProjPos.w;

// 깊이 기반 알파 페이드
    Mask.a *= saturate(fDistance);

// 일정 마스크 이하 픽셀 제거
    if (Mask.r <= 0.4f)
        discard;

// 색상 적용
    float4 Color = float4(250 / 255.0f, 142 / 255.0f, 229 / 255.0f, 1.f);

// 마스크 알파만 곱하고 색상은 그대로 유지
    vColor.rgb *= Color.rgb;
    vColor.a *= Mask.a;

// 결과 출력
    Out.vColor = vColor;

    return Out;
}

struct VS_IN_CHARGE
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float3 vTangent : TANGENT;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT_CHARGE
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};


VS_OUT_CHARGE VS_CHARGE(VS_IN_CHARGE In)
{
    VS_OUT_CHARGE Out;
    
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

struct PS_IN_CHARGE
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

struct PS_OUT_CHARGE
{
    float4 vDiffuse : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
};

PS_OUT_CHARGE PS_CHARGE(PS_IN_CHARGE In)
{
    PS_OUT_CHARGE Out;

    float4 vColor = g_Texture.Sample(DefaultSamplerr, In.vTexcoord);
    float3 Color = float3(255 / 255.f, 255 / 255.f, 255 / 255.f);

    if (vColor.r <= 0.5f)
        discard;

    if (vColor.a <= 0.2f)
        discard;
    
    if (Time <= 0.f)
        discard;
    
    float brightness = dot(vColor.rgb, float3(0.3333, 0.3333, 0.3333));
    if (brightness < 0.1f)
        discard;
    
    vColor.rgb = vColor.rgb + Color.rgb;
    vColor.rgb *= vColor.a;
    Out.vDiffuse = vColor;
    Out.vNormal = float4(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.0f, 0.0f, 0.0f);
    return Out;
}

PS_OUT_CHARGE PS_CHARGEHIT(PS_IN_CHARGE In)
{
    PS_OUT_CHARGE Out;
    float2 UV = In.vTexcoord;

    float2 Center = float2(0.5f, 0.5f);
    UV -= Center;

    float s = sin(Radian * Time);
    float c = cos(Radian * Time);

    float2x2 rMatrix = float2x2(c, -s, s, c);

    UV = mul(UV, rMatrix);

    UV += Center;

    UV = frac(UV);

    float4 vColor = g_Texture.Sample(DefaultSamplerr, UV);
    float3 Color = float3(255 / 255.f, 255 / 255.f, 255 / 255.f);

    if (vColor.r <= 0.5f)
        discard;

    if (vColor.a <= 0.2f)
        discard;
    
    if (Time >= 3.f)
        discard;
    
    float brightness = dot(vColor.rgb, float3(0.3333, 0.3333, 0.3333));
    if (brightness < 0.1f)
        discard;
    
    vColor.rgb = vColor.rgb + Color.rgb;
    vColor.rgb *= vColor.a;
    Out.vDiffuse = vColor;
    Out.vNormal = float4(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.0f, 0.0f, 0.0f);
    return Out;
}

PS_OUT PS_ATTACK(PS_IN_CHARGE In)
{
    PS_OUT Out;
    
    float2 uv = In.vTexcoord;
    float3 Color = float3(1.f, 1.f, 1.f);
    
    if (Dir != 0)
        uv.x = 1.0f - uv.x;
    
    float4 vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, uv);
    
    if (vMtrlDiffuse.r <= 0.0f)
        discard;
    
    if (vMtrlDiffuse.r != 0.f)
        vMtrlDiffuse.rgb += Color;
    
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = float4(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.0f, 0.0f, 0.0f);
    return Out;
}


technique11 DefaultTechnique
{
    pass AngewomonSkill3
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass AngewomonSkillArrow
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Arrow();
    }

    pass AngewomonSkillCross
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Cross();
    }

    pass AngewomonSkill1Effect
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN_SKILL1EFFECT();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SKILL1EFFECT();
    }

    pass CHARGE
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_CHARGE();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_CHARGE();
    }

    pass CHARGEHit
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_CHARGE();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_CHARGEHIT();
    }

    pass Attack
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_ATTACK();
    }
}