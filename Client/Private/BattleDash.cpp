#include "BattleDash.h"
#include "PartObject.h"

CBattleDash::CBattleDash()
{
}

void CBattleDash::Enter(CPartObject* pPart, _uint iIndex, _bool Dash, _bool Loop)
{
	m_pPart = pPart;
	m_bDash = Dash;
	m_bLoop = Loop;
	m_iIndex = iIndex;
}

void CBattleDash::Update(_float fTimedelta)
{
	m_pPart->Set_Animation("BattleDash", m_bLoop);
}

void CBattleDash::Exit()
{
	m_pPart = nullptr;
	m_bDash = false;
	m_bLoop = false;
}

void CBattleDash::Free()
{
	__super::Free();
}
