#pragma once

#include <process.h>
#include "../Default/framework.h"

namespace Client
{
	const unsigned int			g_iWinSizeX = 1280;
	const unsigned int			g_iWinSizeY = 720;

	enum class LEVEL { STATIC, LOADING, LOGO, GAMEPLAY, END };

	enum class ITEMTYPE { WEAPON, SOBY, GITA, END };

	typedef struct Digimon_Data
	{
		wchar_t			Digimon_name[24];
		int				Digimon_ID;
		int				Digimon_Type;
		int				Digimon_Stage;
	};
}

using namespace Client;

extern HINSTANCE g_hInstance;
extern HWND g_hWnd;