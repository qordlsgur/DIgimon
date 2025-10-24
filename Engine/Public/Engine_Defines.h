#ifndef Engine_Define_h__
#define Engine_Define_h__

#include <d3d11.h>
#define DIRECTINPUT_VERSION	0x0800
#include <dinput.h>

#pragma warning(disable : 4251)
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <d3dcompiler.h>
#include "DirectXTK/DDSTextureLoader.h"
#include "DirectXTK/WICTextureLoader.h"

#include "DirectXTK/VertexTypes.h"
#include "DirectXTK/PrimitiveBatch.h"
#include "DirectXTK/Effects.h"

#include "DirectXTK/SpriteBatch.h"
#include "DirectXTK/SpriteFont.h"


#include "FX11/d3dx11effect.h"
using namespace DirectX;

#include <vector>
#include <list>
#include <map>
#include <queue>
#include <algorithm>
#include <functional>
#include <string>
#include <unordered_map>
#include <conio.h>
#include <io.h>
#include <ctime>
#include <iostream>
#include <fstream>
#include <random>

using namespace std;

#include "fmod.h"
#include "fmod.hpp"

#include "Engine_Enum.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_Typedef.h"
#include "Engine_Function.h"

namespace Engine
{
	const static wstring g_strTransformTag = { TEXT("Com_Transform") };

	//const unsigned int g_iMaxWidth = 16384;
	//const unsigned int g_iMaxHeight = 9216;	

	const unsigned int g_iMaxWidth = 8192;
	const unsigned int g_iMaxHeight = 4608;
}

#define DIK_KEY		0xff
#define DIK_MOUSE   3

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif


using namespace Engine;


#endif // Engine_Define_h__
