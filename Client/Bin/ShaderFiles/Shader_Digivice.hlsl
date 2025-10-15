#include "Engine_Shader_Defines.hlsli"
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

Texture2D g_Texture1;
Texture2D g_Texture2;

// : 이 친구는 시메틱 이라고 한다. 선언하는 함수를 보면 시메틱 네임 이라는게 있다.
struct VS_IN // 구조체랑 똑같음 
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT // 위에 IN은 3인데 출력은 무조건 4이다.
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;
    
    // 투영행렬까지 가기 위한 계산을 해 줘야 한다.
    matrix matWV, matWVP;
    
    // mul이라는 함수는 행렬의 곱이 가능하다고 하면 곱하기를 해주는 함수이다.
    matWV = mul(g_WorldMatrix, g_ViewMatrix); // 월드 행렬 * 뷰 행렬
    matWVP = mul(matWV, g_ProjMatrix); // 뷰 스페이스 행렬 * 투영 행렬
    
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    
    Out.vTexcoord = In.vTexcoord;

    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0; // 현재 장치에 바인딩 된 0번째 값
};


// 픽셀 셰이더 함수: 리턴 값은 픽셀의 최종 색
PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;

    // 1번 텍스처 샘플링
    float4 color1 = g_Texture1.Sample(DefaultSampler, In.vTexcoord);

    // 2번 텍스처용 UV와 위치/크기 정의
    float2 tex2Pos = float2(0.04f, 0.02f); // 2번 텍스처 시작 위치 (0~1 범위)
    float2 tex2Size = float2(0.32f, 0.95f); // 2번 텍스처 크기 (0~1 범위)

    // 현재 픽셀이 2번 텍스처 영역 안에 있는지 체크
    float2 localUV = (In.vTexcoord - tex2Pos) / tex2Size;
    bool inside = all(localUV >= 0.0f && localUV <= 1.0f);

    float4 color = color1;

    if (inside)
    {
        // 2번 텍스처 샘플링
        float4 color2 = g_Texture2.Sample(DefaultSampler, localUV);
        if (color2.a > 0)
            color = color2;
    }

    Out.vColor = color;

    if (Out.vColor.a < 0.3f)
        discard;

    return Out;
}

technique11 DefaultTechnique
{
		// pass도 여러개 정의 가능하다.
    pass DefaultPass
    {
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}