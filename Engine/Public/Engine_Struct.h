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
	typedef struct tagVertexPositionTexcoord
	{
		XMFLOAT3			vPosition;
		XMFLOAT2			vTexcoord;
	}VTXPOSTEX;

}



#endif // Engine_Struct_h__
