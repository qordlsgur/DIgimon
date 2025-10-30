#include "Engine_Shader_Defines.hlsli"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
Texture2D g_DiffuseTexture;
vector g_Scale;
matrix g_vCamPosition;

Texture2D g_Texture;
Texture2D g_DepthTexture;
Texture2D g_Dissolve;
Texture2D g_Mask;

float Time;
int Count;
float Frame;
float Radian;

float g_color;

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
    float3 vColor = float3(254, 253, 72) / 255.f;

    
    //if (vMtrlDiffuse.a <= 0.f)
    //    discard;
    
    vMtrlDiffuse.rgb = vMtrlDiffuse.rgb + vColor.rgb;
    
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = float4(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.0f, 0.0f, 0.0f);
    return Out;
}

struct VS_IN_4X4
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float3 vTangent : TANGENT;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT_4X4
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};


VS_OUT_4X4 VS_FIRE4X4(VS_IN_4X4 In)
{
    VS_OUT_4X4 Out;
    
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

struct PS_IN_4X4
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

struct PS_OUT_4X4
{
    float4 vDiffuse : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
};

PS_OUT_4X4 PS_FIRE4X4(PS_IN_4X4 In)
{
    PS_OUT_4X4 Out;
    
    int totalFrame = Count * Count;
    
    int lastFrame = totalFrame * Frame;
    
    int currentFrame = int(floor(Time / Frame)) % totalFrame;

    int frameX = currentFrame % Count;
    int frameY = currentFrame / Count;

    float frameWidth = 1.0 / Count;
    float frameHeight = 1.0 / Count;
    
    
    float2 uv = In.vTexcoord;
    uv.y = 1.f - uv.y;
    uv.x = uv.x * frameWidth + frameX * frameWidth;
    uv.y = uv.y * frameHeight + frameY * frameHeight;
    
    float4 color = g_Texture.Sample(DefaultSampler, uv);
 
    if (color.r <= 0.3f)
        discard;
    
    if (lastFrame <= Time)
        discard;
    
    color.a *= 0.7f;
    
    Out.vDiffuse = color;
    Out.vNormal = float4(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.0f, 0.0f, 0.0f);
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
    
    float2 uv = In.vTexcoord;
    
    uv = uv - 0.5f;
    
    float sinR = sin(Radian * Time);
    float cosR = cos(Radian * Time);
    
    float2 Rotated;
    
    Rotated.x = uv.x * cosR - uv.y * sinR;
    Rotated.y = uv.x * sinR + uv.y * cosR;
    
    Rotated += 0.5f;
    
    float4 vColor = g_Texture.Sample(DefaultSampler, Rotated);
    float4 Color = float4(254 / 255.f, 253 / 255.f, 72 / 255.f, vColor.a);

    if (vColor.r <= 0.3f)
        discard;
    
    if (Time >= 3.f)
        discard;
        
    Color.a = vColor.a;
    
    Out.vDiffuse = Color;
    Out.vNormal = float4(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.0f, 0.0f, 0.0f);
    return Out;
}

struct VS_IN_BITE
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float3 vTangent : TANGENT;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT_BITE
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

VS_OUT_BITE VS_BITE(VS_IN_BITE In)
{
    VS_OUT_BITE Out;
  
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

struct PS_IN_BITE
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

struct PS_OUT_BITE
{
    float4 vDiffuse : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
};

/* 픽셀 쉐이더 : 픽셀의 최종적인 색을 결정하낟. */
PS_OUT_BITE PS_BITE(PS_IN_BITE In)
{
    PS_OUT_BITE Out;
    
    float2 uvRotated = float2(1.0f - In.vTexcoord.y, In.vTexcoord.x);
    
    float4 vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, uvRotated);
    float4 Color = g_Mask.Sample(DefaultSampler, In.vTexcoord);
    
    if (vMtrlDiffuse.r <= 0.f)
        discard;
    
    vMtrlDiffuse = vMtrlDiffuse * Color;
    
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = float4(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.0f, 0.0f, 0.0f);
    return Out;
}

PS_OUT_4X4 PS_BITEEFFECT(PS_IN_4X4 In)
{
    PS_OUT_4X4 Out;
    
    int totalFrame = Count * Count;
    
    int lastFrame = totalFrame * Frame;
    
    int currentFrame = int(floor(Time / Frame)) % totalFrame;

    int frameX = currentFrame % Count;
    int frameY = currentFrame / Count;

    float frameWidth = 1.0 / Count;
    float frameHeight = 1.0 / Count;
    
    
    float2 uv = In.vTexcoord;
    uv.y = 1.f - uv.y;
    uv.x = uv.x * frameWidth + frameX * frameWidth;
    uv.y = uv.y * frameHeight + frameY * frameHeight;
    
    float4 vMtrlDiffuse = g_Texture.Sample(DefaultSampler, uv);
    float4 Color = g_Mask.Sample(DefaultSampler, In.vTexcoord);
    
    if (vMtrlDiffuse.r <= 0.3f)
        discard;
    
    if (lastFrame <= Time)
        discard;
    
    vMtrlDiffuse = vMtrlDiffuse * Color;
    
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
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_LASER();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_GLASER();
    }

    pass Fire4x4
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_FIRE4X4();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_FIRE4X4();
    }

    pass CHARGE
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_CHARGE();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_CHARGE();
    }

    pass Bite
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_BITE();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_BITE();
    }

    pass BiteEffect
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_FIRE4X4();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_BITEEFFECT();
    }
}