#include "StateMachine.h"
#include "State.h"
#include "PartObject.h"

#include "BattleBack.h"
#include "BattleDash.h"
#include "Death.h"
#include "Fail.h"
#include "Hit.h"
#include "LookAround.h"
#include "Roar.h"
#include "Run.h"
#include "Skill1.h"
#include "Skill2.h"
#include "Skill3.h"
#include "Stand.h"
#include "StandBattle.h"
#include "Jump.h"

CStateMachine::CStateMachine()
{
	BattleBack = new CBattleBack();
	BattleDash = new CBattleDash();
	Death = new CDeath();
	Fail = new CFail();
	Hit = new CHit();
	LookAround = new CLookAround();
	Roar = new CRoar();
	Run = new CRun();
	Skill1 = new CSkill1();
	Skill2 = new CSkill2();
	Skill3 = new CSkill3();
	Stand = new CStand();
	StandBattle = new CStandBattle();
	Jump = new CJump();
}

HRESULT CStateMachine::Initialize()
{
	return S_OK;
}

void CStateMachine::Enter(DIGIMONSTATE eNonBattleState, CPartObject* pPart, _bool Dash, _bool Loop)
{
	if (Search(eNonBattleState) == false)
		return;

	if (m_pCurrentState)
	{
		m_pCurrentState->Exit();
		m_bDash = false;
		m_pPart = nullptr;
		m_pCurrentState = nullptr;
	}

	switch (eNonBattleState)
	{
	case DIGIMONSTATE::BATTLEBACK:
		m_pCurrentState = BattleBack;
		m_bBattle = true;
		break;

	case DIGIMONSTATE::BATTLEDASH:
		m_pCurrentState = BattleDash;
		m_bBattle = true;
		break;

	case DIGIMONSTATE::DEATH:
		m_pCurrentState = Death;
		m_bBattle = true;
		break;

	case DIGIMONSTATE::FAIL:
		m_pCurrentState = Fail;
		m_bBattle = true;
		break;

	case DIGIMONSTATE::HIT:
		m_pCurrentState = Hit;
		m_bBattle = true;
		break;

	case DIGIMONSTATE::LOOKAROUND:
		m_pCurrentState = LookAround;
		m_bBattle = false;
		break;

	case DIGIMONSTATE::ROAR:
		m_pCurrentState = Roar;
		m_bBattle = false;
		break;

	case DIGIMONSTATE::RUN:
		m_pCurrentState = Run;
		m_bBattle = false;
		break;

	case DIGIMONSTATE::SKILL1:
		m_pCurrentState = Skill1;
		m_bBattle = true;
		break;

	case DIGIMONSTATE::SKILL2:
		m_pCurrentState = Skill2;
		m_bBattle = true;
		break;

	case DIGIMONSTATE::SKILL3:
		m_pCurrentState = Skill3;
		m_bBattle = true;
		break;

	case DIGIMONSTATE::STAND:
		m_pCurrentState = Stand;
		m_bBattle = false;
		break;

	case DIGIMONSTATE::STANDBATTLE:
		m_pCurrentState = StandBattle;
		m_bBattle = true;
		break;
	case DIGIMONSTATE::JUMP:
		m_pCurrentState = Jump;
		m_bBattle = false;
		break;
	}

	m_bDash = Dash;
	m_pPart = pPart;
	m_bLoop = Loop;
	m_pPart->Set_State(eNonBattleState);
	m_pCurrentState->Enter(m_pPart, ENUM_CLASS(eNonBattleState), m_bDash, m_bLoop);
}

void CStateMachine::Update(_float fTimedelta)
{
	if (m_pCurrentState)
		m_pCurrentState->Update(fTimedelta);

	if (m_pPart->Get_AnimFinish())
	{
		if (!m_bBattle)
		{
			Enter(DIGIMONSTATE::STAND, m_pPart, false, true);
		}
		else if (m_bBattle)
		{
			Enter(DIGIMONSTATE::STANDBATTLE, m_pPart, false, true);
		}
	}
}

_bool CStateMachine::Search(DIGIMONSTATE eNonBattleState)
{
		switch (eNonBattleState)
	{
	case DIGIMONSTATE::BATTLEBACK:
	case DIGIMONSTATE::BATTLEDASH:
	case DIGIMONSTATE::DEATH:
	case DIGIMONSTATE::FAIL:
	case DIGIMONSTATE::HIT:
	case DIGIMONSTATE::LOOKAROUND:
	case DIGIMONSTATE::ROAR:
	case DIGIMONSTATE::RUN:
	case DIGIMONSTATE::SKILL1:
	case DIGIMONSTATE::SKILL2:
	case DIGIMONSTATE::SKILL3:
	case DIGIMONSTATE::STAND:
	case DIGIMONSTATE::STANDBATTLE:
	case DIGIMONSTATE::JUMP:
		return true;
	default:
		return false;
	}

}


CStateMachine* CStateMachine::Create()
{
	CStateMachine* pInstance = new CStateMachine();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CStateMachine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateMachine::Free()
{
	__super::Free();

	Safe_Delete(BattleBack);
	Safe_Delete(BattleDash);
	Safe_Delete(Death);
	Safe_Delete(Fail);
	Safe_Delete(Hit);
	Safe_Delete(LookAround);
	Safe_Delete(Roar);
	Safe_Delete(Run);
	Safe_Delete(Skill1);
	Safe_Delete(Skill2);
	Safe_Delete(Skill3);
	Safe_Delete(Stand);
	Safe_Delete(StandBattle);
	Safe_Delete(Jump);

	Safe_Release(m_pCurrentState);
}
