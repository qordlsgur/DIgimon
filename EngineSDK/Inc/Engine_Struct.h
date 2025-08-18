#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef struct tagEngineDesc
	{
		HINSTANCE			hInstance;
		HWND				hWnd;
		unsigned int		iNumLevels;
		unsigned int		iWinSizeX, iWinSizeY;
		WINMODE				eWindowMode;
	}ENGINE_DESC;

	// 구조체
	// 이 구조체는 VertexPosition이랑 TexCoord를 정의 해주는 구조체이다.
	// 원래는 밑에 vPosition이랑 vTexCoord밖에 없었는데 셰이더를 추가 할 때마다 계속
	// 밑에 static으로 되어있는 2개를 불러 줘야 했다.
	// 하지만 그걸 계속하다가 까먹으면 또 셰이더에서 오류가 나서 아예 직접 추가를 해줬다.
	// 그냥 추가를 했을때는 VTXPOSTEX pvt~이렇게 선언을 해야 실행이 된다.
	// 그리고 구조체의 size마저 늘어나게 된다. 그거를 방지 하기 위해서 static const를 이용해
	// 선언을 안하고 컴파일러 단계에서 할당이 되어서 바로 사용이 가능하다.
	// int 나 그런건 되는데 배열은 또 초기화가 안된다.
	// 11버전에 추가가 된건데 constexpr을 이용하면 이제 배열도 가능해졌다.
	typedef struct tagVertexPositionTexcoord
	{
		XMFLOAT3			vPosition;
		XMFLOAT2			vTexcoord;

		static constexpr unsigned int					iNumElements = { 2 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

	}VTXPOSTEX;

	// 위에는 이제 위치랑 텍스쳐만 할 수 있다면 이번에는 위치랑 텍스쳐 그리고 법선까지 추가됨
	typedef struct tagVertexPositionNormalTexcoord
	{
		XMFLOAT3			vPosition;
		XMFLOAT3			vNormal;
		XMFLOAT2			vTexcoord;

		static constexpr unsigned int					iNumElements = { 3 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
	}VTXNORTEX;

}

#endif // Engine_Struct_h__
