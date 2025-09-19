#include "BattleBack.h"
#include "PartObject.h"

CBattleBack::CBattleBack()
{
}

void CBattleBack::Enter(CPartObject* pPart, _uint iIndex, _bool Dash, _bool Loop)
{
	m_pPart = pPart;
	m_bDash = Dash;
	m_bLoop = Loop;
	m_iIndex = iIndex;
}

void CBattleBack::Update(_float fTimedelta)
{
	m_pPart->Set_Animation("BattleBack", m_bLoop);
}

void CBattleBack::Exit()
{
	m_pPart = nullptr;
	m_bDash = false;
	m_bLoop = false;
}

void CBattleBack::Free()
{
	__super::Free();
}
