// 하나의 셰디어 파일 안에 다양한 셰이딩을 함
// 어떤 셰이더가 어떤 버전으로 할 지 선언도 해야 함
// 정점 셰이더 함수에 대한 설명이다.
// 정점에 대한 셰이딩 = 정점에 필요한 연산을 수행 = 정점의 기본적인 상캐변환(월드, 뷰, 투영
// 등등) + 추가 변환 
// 저거를 해 주기 위해서는 파라미터로 정점을 받아와야 해서 구조체로 다 받아옴
// 정점 셰이더의 기능
// 1. 정점 바꾸기
// 2. 정점의 "구성 정보"를 수정 또는 변경이 가능하다.
// 셰이더의 코드는 C와 유사하다.
// 셰이더는 변수를 float, flaot2, float3,float4 (vector로 대체 가능) 
// matrix, float4x4, float1x3, float3x3등 많이 있다. 
// 그리고 float3 vTmp 로 선언을 하면
// vTmp.x = 0.f, vTmp.xy 1.f, vTmp = 1.f, float3(0.f,0.f,0.f)등등 많은게 가능하다.(r,g,b)
// 밑에 적은 친구들을 전역변수로 받아올 수 있는데 받아오면 상수값이라서 변경이 안됨.
// 그 밑에다가 값을 바꾸는거도 안되고 상수라서 안바뀜
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
// 이 친구들은 GPU에 선언이 되어서 Vram에 저장이 된다. 그래서 사상 메모리를 사용 못함.
// vram의 용량에는 한계가 있어서 이름을 가급적이면 통일을 해 주는게 좋다.
// 이름을 통일하면 vram을 안먹지만 다르면 새로 할당을 함 그래서 GPU용량 이상이면 터진다.
// 그리고 값이 한번 저장이 되면 그 값을 선언한 모든 곳에서 사용이 가능함.
// 그래서 값을 매번 바꿔 줘야한다. 

texture2D g_Texture;

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
// 입력과 출력 둘 다 정점이다.
// 함수랑 똑같음 앞에 VS_OUT은 리턴 할 때 float int void 붙이는 느낌이다.
// 함수의 이름을 적고 매개변수로 수정할 친구를 넣어줌

// 버텍스 셰이더 루틴
// 삼각형을 그리면 0,1,2로 꺼내서 1개씩 전달해서 3번 호출함 
// 하지만 GPU는 저성능이지만 벙렬로 해줘서 한번에 계산을 해줌
// 0번꺼내고 계산 1번 꺼내고 계산이 아니라
// 0,1,2 한번에 꺼내서 한번에 계산함
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
    float4 vColor : SV_TARGET0; // 현재 장치에 바인딩 된 0번째 값
};


// 픽셀 셰이더 함수: 리턴 값은 픽셀의 최종 색
PS_OUT PS_MAIN(PS_IN In)
{
	// 출력을 위한 변수
    
    PS_OUT Out;

    Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
    
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
		  //{
  //    VertexShader = compile (버전 지정) 
  //    vs_5_0(외우면됨)
  //    VS_MAIN() 이친구를 이용해서  사용함(진입점함수);
  //    PixelShader = compile ps_5_0 PS_MAIN();
  //}

 //블렌딩
 //pass DefaultPass1
 //{
 //    VertexShader = compile vs_5_0 VS_MAIN1();
 //    PixelShader = compile ps_5_0 PS_MAIN1();
 //}

 //왜곡
 //pass DefaultPass2
 //{
 //    VertexShader = compile vs_5_0 VS_MAIN2();
 //    PixelShader = compile ps_5_0 PS_MAIN2();
 //}
 // 이렇게 여러개 정의를 해 두고 if else처럼 분기를 두고 할 수 있다.
}

// 옛날에는 셰이더 버전을 지원을 해주는 버전이 다르면 실행이 안되서 여러개 정의 함
// 근데 요즘은 5버전이 안되는 컴이 거의 찾아보기 힘들정도라 안해줘도 상관이 없음
// 그게 아니라 techinque11 DefaultTechnique이 친구도 여러개 정의 가능하다.
// techinque11 DefaultTechnique: (밑에 적음) 여러개 정의 가능
// {
// pass DefaultPass 
// {
//	 VertexShader = compile vs_4_0 VS_MAIN();
//	 PixelShader = compile ps_4_0 PS_MAIN();
// } 

// }

// techinque11 DefaultTechnique: (밑에 적음) 여러개 정의 가능
// {
// pass DefaultPass 
// {
//	 VertexShader = compile vs_3_0 VS_MAIN();
//	 PixelShader = compile ps_3_0 PS_MAIN();
// }
