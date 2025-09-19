#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "PartObject.h"

NS_BEGIN(Engine)
class CPartObject;
NS_END

NS_BEGIN(Client)

class CStateMachine final : public CBase
{
public:
	CStateMachine();
	virtual ~CStateMachine() = default;
public:
	HRESULT Initialize();
	void Enter(DIGIMONSTATE eNonBattleState, CPartObject* pPart, _bool Dash = true, _bool Loop = true);
	void Update(_float fTimedelta);

private:
	_bool Search(DIGIMONSTATE eNonBattleState);

private:
	class CBattleBack* BattleBack = { nullptr };
	class CBattleDash* BattleDash = { nullptr };
	class CDeath* Death = { nullptr };
	class CFail* Fail = { nullptr };
	class CHit* Hit = { nullptr };
	class CLookAround* LookAround = { nullptr };
	class CRoar* Roar = { nullptr };
	class CRun* Run = { nullptr };
	class CSkill1* Skill1 = { nullptr };
	class CSkill2* Skill2 = { nullptr };
	class CSkill3* Skill3 = { nullptr };
	class CStand* Stand = { nullptr };
	class CStandBattle* StandBattle = { nullptr };
	class CJump* Jump = { nullptr };

	CPartObject*					m_pPart = { nullptr };

	_bool							m_bDash = { false };
	_bool							m_bLoop = { false };
	_bool							m_bBattle = { false };
	class CState*					m_pCurrentState = { nullptr };

public:
	static CStateMachine* Create();
	virtual void Free() override;

};

NS_END