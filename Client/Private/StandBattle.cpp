#include "StandBattle.h"
#include "PartObject.h"

CStandBattle::CStandBattle()
{
}

void CStandBattle::Enter(CPartObject* pPart, _uint iIndex, _bool Dash, _bool Loop)
{
	m_pPart = pPart;
	m_bDash = Dash;
	m_bLoop = Loop;
	m_iIndex = iIndex;
}

void CStandBattle::Update(_float fTimedelta)
{
	m_pPart->Set_Animation("StandBattle", m_bLoop);
}

void CStandBattle::Exit()
{
	m_pPart = nullptr;
	m_bDash = false;
	m_bLoop = false;
}

void CStandBattle::Free()
{
	__super::Free();
}
