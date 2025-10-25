#include "Engine_Shader_Defines.hlsli"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_DiffuseTexture;

texture2D g_Texture;
texture2D g_DepthTexture;
texture2D g_Dissolve;
texture2D g_Mask;

float Time;

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
    
    if (vMtrlDiffuse.r <= 0.4f)
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
    
    float alpha = vMtrlDiffuse.a;
   
    
    if (vMtrlDiffuse.r <= 0.2f)
        discard;
    
    if (Time > Disslove.r)
        discard;
    
    vMtrlDiffuse.rgb = vMtrlDiffuse.rgb * Color;
    
    vMtrlDiffuse *= Disslove.a;
    
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
    
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    
    /* Out.vPosition.xy => 시야각에 있는 점들을 90에 맞춰준다 */ 
    /* Out.vPosition.z => n~f사이에 있는 점들의 z를 0 ~ f로 바꿔준다. */     
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
    
    Mask.a = Mask.a * saturate(fDistance);
    
    if (Mask.r <= 0.4f)
        discard;
    
    float4 Color = float4(250 / 255.0f, 142 / 255.0f, 229 / 255.0f, Mask.a);
    
    Mask *= Color;
    
    //vColor *= Mask;
    
    Out.vColor = Mask;
    
    return Out;
}


//struct VS_OUT_CROSS_EFFECT
//{
//    float4 vPosition : SV_POSITION;
//    float2 vTexcoord : TEXCOORD0;
//    float4 vProjPos : TEXCOORD1;
//};

//VS_OUT_CROSS_EFFECT VS_MAIN_CROSS_EFFECT(VS_IN In)
//{
//    VS_OUT_CROSS_EFFECT Out;
    
//    matrix matWV, matWVP;
    
//    matWV = mul(g_WorldMatrix, g_ViewMatrix);
//    matWVP = mul(matWV, g_ProjMatrix);
    
//    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    
//    /* Out.vPosition.xy => 시야각에 있는 점들을 90에 맞춰준다  
//    /* Out.vPosition.z => n~f사이에 있는 점들의 z를 0 ~ f로 바꿔준다. */     
//    Out.vTexcoord = In.vTexcoord;
//    Out.vProjPos = Out.vPosition;

//    return Out;
//}

//struct PS_IN_CROSS_EFFECT
//{
//    float4 vPosition : SV_POSITION;
//    float2 vTexcoord : TEXCOORD0;
//    float4 vProjPos : TEXCOORD1;
//};

//struct PS_OUT_CROSS_EFFECT
//{
//    float4 vColor : SV_TARGET0;
//};

//PS_OUT_CROSS_EFFECT PS_MAIN_CROSS_EFFECT(PS_IN_CROSS_EFFECT In)
//{
//    PS_OUT_CROSS_EFFECT Out;
    
//    float4 vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
//    float4 Mask = g_Mask.Sample(DefaultSampler, In.vTexcoord);
//    float2 vTexcoord;
    
//    vTexcoord.x = In.vProjPos.x / In.vProjPos.w * 0.5f + 0.5f;
//    vTexcoord.y = In.vProjPos.y / In.vProjPos.w * -0.5f + 0.5f;
    
//    float4 vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vTexcoord);
    
//    float fOldViewZ = vDepthDesc.y * 500.f;
    
//    float fDistance = fOldViewZ - In.vProjPos.w;
    
//    Mask.a = Mask.a * saturate(fDistance);
    
//    if (Mask.r <= 0.4f)
//        discard;
    
//    float4 Color = float4(250 / 255.0f, 142 / 255.0f, 229 / 255.0f, Mask.a);
    
//    Mask *= Color;
    
//    Out.vColor = Mask;
    
//    return Out;
//}

technique11 DefaultTechnique
{
    pass AngewomonSkill3
    {
        SetRasterizerState(RS_Default);
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
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
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
}