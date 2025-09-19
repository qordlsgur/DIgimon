#include "Stand.h"
#include "PartObject.h"

CStand::CStand()
{
}

void CStand::Enter(CPartObject* pPart, _uint iIndex, _bool Dash, _bool Loop)
{
	m_pPart = pPart;
	m_bDash = Dash;
	m_bLoop = Loop;
	m_iIndex = iIndex;
}

void CStand::Update(_float fTimedelta)
{
	m_pPart->Set_Animation("Stand", m_bLoop);
}

void CStand::Exit()
{
	m_pPart = nullptr;
	m_bDash = false;
	m_bLoop = false;
}

void CStand::Free()
{
	__super::Free();
}
