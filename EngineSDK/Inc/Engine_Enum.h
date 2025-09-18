#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum class WINMODE { FULL, WIN, END };	
	enum class PROTOTYPE { GAMEOBJECT, COMPONENT };
	enum class STATE { RIGHT, UP, LOOK, POSITION, END };
	enum class LIGHT { DIRECTIONAL, POINT, SPOT, END };
	enum class D3DTS { VIEW, PROJ, END };
	enum class MODEL { ANIM, NONANIM };
	enum class RENDER { PRIORITY, NONBLEND, BLEND, UI, BLENDUI, END};
	enum class TEXTURE { PLANE, CUBE, END };
	enum class RAY { LOCAL, WORLD, END };
	enum class CHANNELID { EFFECT, BGM, END };
	enum class MOVEOBJECT { GO, BACK, LEFT, RIGHT };
	enum class MOUSEKEYSTATE { LBUTTON, RBUTTON, WBUTTON, XBUTTON };
	enum class MOUSEMOVESTATE { X, Y, W };
	enum class DIGIMONSTATE { BATTLEBACK = 0, BATTLEDASH, DEATH, FAIL, HIT, LOOKAROUND, ROAR, RUN, SKILL1, SKILL2, SKILL3, STAND, STANDBATTLE, JUMP, END };
	enum class DIGIMONSTATE2 { BATTLEBACK = 0, BATTLEDASH, DEATH, FAIL, HIT, LOOKAROUND, ROAR, RUN, SKILL1, SKILL2, STAND, STANDBATTLE };
	enum class PLAYER_STATE { STAND, MOVE };
	enum class NAVI_POINT { A, B, C, END };
	enum class NAVI_LINE { AB, BC, CA, END };
}
#endif // Engine_Enum_h__
