matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;


texture2D g_Texture1;
texture2D g_Texture2;

sampler DefaultSampler = sampler_state
{
    Filter = MIN_MAG_MIP_LINEAR;
    
};

// : 이 친구는 시메틱 이라고 한다. 선언하는 함수를 보면 시메틱 네임 이라는게 있다.
struct VS_IN // 구조체랑 똑같음 
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    // 만약 Texcoord가 더 있으면 float2 vTexcoord : TEXCOORD1,2 이렇게 붙이면 된다.
};

struct VS_OUT // 위에 IN은 3인데 출력은 무조건 4이다.
{
		// SV는 셰이더 벨류로 이거를 붙이면 내가 할건 다 했으니 이제 장치보고 알아서
		// 하라는 뜻이다. 그래서 이거를 붙이면 이제 w나누기 까지 해주는데 안붙이면 안해줌
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
    
    // 밑에 계산식은 1*3이랑 4*4 라서 계산을 하기 위해서 뒤에 w값인 1을 넣어준다.
    // 여기서 왜 w가 0인가 1인가는 렌더링 파이프라인에 투영행렬을 해 주는 과정에 설명
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    
    Out.vTexcoord = In.vTexcoord;

    return Out;
}

// 위에서 return 까지 해 버리면 이제 SV가 붙어 있으면 w나누기 까지 다 해준다.
// 위에서 계산 한거는 말 그대로 곱하기만 해줘서 w나누기는 알아서 해준다.

struct PS_IN // 이 친구는 w나누기까지 다 한 값을 들고옴
{
    float4 vPosition : SV_POSITION; // 윈도우 상의 좌표이다(모든 계산이 끝난 친구)
    float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
    float4 vColor1 : SV_TARGET0; // 현재 장치에 바인딩 된 0번째 값
    float4 vColor2 : SV_TARGET1; // 현재 장치에 바인딩 된 1번째 값
};


// 픽셀 셰이더 함수: 리턴 값은 픽셀의 최종 색
PS_OUT PS_MAIN(PS_IN In)
{
	// 출력을 위한 변수
    
    PS_OUT Out;

    Out.vColor1 = g_Texture1.Sample(DefaultSampler, In.vTexcoord);
    Out.vColor2 = g_Texture2.Sample(DefaultSampler, In.vTexcoord);
    
    return Out;
}

// techinque11 DefaultTechnique: (밑에 적음) 
// 여러개 정의 가능(그 이유은 평범하게 셰이더 버전을 낮은 버전도 적어놓음)
// 필요없음 아무리 오래 되어도 5.0지원을 해줌
// 이 친구도 구조체 처럼 적으면된다, 이름은 마음대로

technique11 DefaultTechnique
{
		// pass도 여러개 정의 가능하다.
    pass DefaultPass
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}