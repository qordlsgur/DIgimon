#include "Engine_Shader_Defines.hlsli"
#include "Effect.hlsli"
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
vector g_Scale;
vector g_vCamPosition;
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
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;
    
    
    /* In.vPosition * 월드 * 뷰 * 투영 */    
    //float4x4 == matrix
    matrix matWV, matWVP;
    
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    
    /* Out.vPosition.xy => 시야각에 있는 점들을 90에 맞춰준다 */ 
    /* Out.vPosition.z => n~f사이에 있는 점들의 z를 0 ~ f로 바꿔준다. */     
    Out.vTexcoord = In.vTexcoord;

    return Out;
}

/* 출력된 정점 위치벡터의 w값으로 모든 성분을 나눈다 -> 투영스페이스로 변환 */ 
/* 정점의 위치에 대해서 뷰포트 변환을 수행한다 */ 
/* 정점의 모든 정보를 보간하여 픽셀을 만든다. -> 래스터라이즈 */ 

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};

/* 픽셀 쉐이더 : 픽셀의 최종적인 색을 결정하낟. */
PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;
    
    Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
    if (Out.vColor.a == 0.f)
        discard;
    return Out;
}

struct VS_OUT_SOFTEFFECT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
};

VS_OUT_SOFTEFFECT VS_MAIN_SOFTEFFECT(VS_IN In)
{
    VS_OUT_SOFTEFFECT Out;
    
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

struct PS_IN_SOFTEFFECT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
};

PS_OUT PS_MAIN_SOFTEFFECT(PS_IN_SOFTEFFECT In)
{
    PS_OUT Out;
    
    float4 vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
    
    if (vColor.a == 0.f)
        discard;
    
    float2 vTexcoord;
    
    vTexcoord.x = In.vProjPos.x / In.vProjPos.w * 0.5f + 0.5f;
    vTexcoord.y = In.vProjPos.y / In.vProjPos.w * -0.5f + 0.5f;
    
    float4 vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vTexcoord);
    
    float fOldViewZ = vDepthDesc.y * 500.f;
    
    float fDistance = fOldViewZ - In.vProjPos.w;
    
    vColor.a = vColor.a * saturate(fDistance);
    
    Out.vColor = vColor;
    
    return Out;
}

struct VS_OUT_TEST
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
};

VS_OUT_TEST VS_MAIN_TEST(VS_IN In)
{
    VS_OUT_TEST Out;
    
    matrix matWV, matWVP;
    
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    float2 Offset = In.vTexcoord - 0.5f; // (-0.5 ~ 0.5)

    float3 vLook = normalize(g_vCamPosition - In.vPosition);
    float3 rightDir = normalize(cross(float3(0.f, 1.f, 0.f), vLook));
    float3 upDir = normalize(cross(vLook, rightDir));

    float3 dirRight = rightDir * g_Scale.x * Offset.x;
    float3 dirUp = upDir * g_Scale.y * Offset.y;
    
    float3 worldPos = In.vPosition + dirRight + dirUp ;

    Out.vPosition = mul(float4(worldPos, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;
    Out.vProjPos = Out.vPosition;

    return Out;
}

struct PS_IN_TEST
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
};

struct PS_OUT_TEST
{
    float4 vColor : SV_TARGET0;
};

PS_OUT PS_MAIN_TEST(PS_IN_TEST In)
{
    PS_OUT_TEST Out;
   
    float4 vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
    
    if (vColor.a == 0.f)
        discard;
    
    float2 vTexcoord;
    
    vTexcoord.x = In.vProjPos.x / In.vProjPos.w * 0.5f + 0.5f;
    vTexcoord.y = In.vProjPos.y / In.vProjPos.w * -0.5f + 0.5f;
    
    float4 vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vTexcoord);
    
    float fOldViewZ = vDepthDesc.y * 500.f;
    
    float fDistance = fOldViewZ - In.vProjPos.w;
    
    vColor.a = vColor.a * saturate(fDistance);
    
    Out.vColor = vColor;
    
    return Out;
    
     
    //float4 vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
    //float4 Mask = g_Mask.Sample(DefaultSampler, In.vTexcoord);
    //float2 vTexcoord;
    
    
    //vTexcoord.x = In.vProjPos.x / In.vProjPos.w * 0.5f + 0.5f;
    //vTexcoord.y = In.vProjPos.y / In.vProjPos.w * -0.5f + 0.5f;
    
    //float4 vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vTexcoord);
    
    //float fOldViewZ = vDepthDesc.y * 500.f;
    
    //float fDistance = fOldViewZ - In.vProjPos.w;
    
    //Mask.a = Mask.a * saturate(fDistance);
    
    //if (Mask.r <= 0.4f)
    //    discard;
    
    //float4 Color = float4(250 / 255.0f, 142 / 255.0f, 229 / 255.0f, Mask.a);
    
    //Mask *= Color;
    
    ////vColor *= Mask;
    
    //Out.vColor = vColor;
}

technique11 DefaultTechnique
{
    pass UI
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    } 

    pass SoftEffect
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN_SOFTEFFECT();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SOFTEFFECT();
    }

    pass Blur
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass Test
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN_TEST();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_TEST();
    }
}