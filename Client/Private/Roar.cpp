#include "Roar.h"
#include "PartObject.h"

CRoar::CRoar()
{
}

void CRoar::Enter(CPartObject* pPart, _uint iIndex, _bool Dash, _bool Loop)
{
	m_pPart = pPart;
	m_bDash = Dash;
	m_bLoop = Loop;
	m_iIndex = iIndex;
}

void CRoar::Update(_float fTimedelta)
{
	m_pPart->Set_Animation("Roar", m_bLoop);
}

void CRoar::Exit()
{
	m_pPart = nullptr;
	m_bDash = false;
	m_bLoop = false;
}

void CRoar::Free()
{
	__super::Free();
}
