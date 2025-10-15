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
	enum class RENDER { PRIORITY, NONBLEND, NONLIGHT, BLEND, UI, BLENDUI, END};
	enum class TEXTURE { PLANE, CUBE, END };
	enum class RAY { LOCAL, WORLD, END };
	enum class CHANNELID { EFFECT, BGM, END };
	enum class MOVEOBJECT { GO, BACK, LEFT, RIGHT };
	enum class MOUSEKEYSTATE { LBUTTON, RBUTTON, WBUTTON, XBUTTON };
	enum class MOUSEMOVESTATE { X, Y, W };
	enum class DIGIMONSTATE { BATTLEBACK = 0, BATTLEDASH, DEATH, FAIL, HIT, LOOKAROUND, ROAR, RUN, SKILL1, SKILL2, SKILL3, STAND, STANDBATTLE, JUMP, END };
	enum class PLAYER_STATE { STAND, MOVE };
	enum class NAVI_POINT { A, B, C, END };
	enum class NAVI_LINE { AB, BC, CA, END };
	enum class DIGIMON_STAGE {INTAINING, ROOKIE, CHAMPION, ULTIMATE, MEGA, END};
	enum class DIGIMON_ATTRIBUTE {DATA, VIRUS, VACCINE, END};
	enum class BATTLE_STATE { START, SKILL, ING, END };
	enum class SKILL_TARGET { SELF, ALLY, ENEMY, END};
	enum class SKILL_RANGE { SINGLE, ALL, END};
}
#endif // Engine_Enum_h__
