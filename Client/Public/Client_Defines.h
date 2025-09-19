#pragma once

#include <process.h>
#include "../Default/framework.h"

namespace Client
{
	const unsigned int			g_iWinSizeX = 1280;
	const unsigned int			g_iWinSizeY = 720;

	enum class LEVEL { STATIC, LOADING, LOGO, GAMEPLAY, END };

	enum class ITEMTYPE { WEAPON, SOBY, GITA, END };

	enum class OBJECT {
		TREE1, TREE2, TREE3, TENNIS, CAFE, BILDING1, BILDING2, BILDING3, BILDING4
		, BILDING5, BILDING6, BILDING7, BILDING8, BILDING9, BILDING10, BILDING11, BILDING12, BILDING13
	};

}

using namespace Client;

extern HINSTANCE g_hInstance;
extern HWND g_hWnd;