matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

Texture2D g_Texture;

sampler DefaultSampler = sampler_state
{
    Filter = MIN_MAG_MIP_LINEAR;
    
};

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
	
		// 받아온 정점의 IN.vPotition * 월드 * 뷰 * 투영
	
		// return에 필요한 변수이다.
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
    
    float brightness = 10.f;
    
    float4 SampleColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
    
    // 값이 1.0 이상으로 올라가지 않게 saturate로 클램핑
    float3 adjustedColor = saturate(SampleColor.rgb * brightness);
    
    Out.vColor = float4(adjustedColor, SampleColor.a);
    //Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
    
    return Out;
}

technique11 DefaultTechnique
{
		// pass도 여러개 정의 가능하다.
    pass DefaultPass
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}