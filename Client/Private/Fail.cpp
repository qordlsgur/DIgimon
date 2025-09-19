#include "Fail.h"
#include "PartObject.h"
CFail::CFail()
{
}

void CFail::Enter(CPartObject* pPart, _uint iIndex, _bool Dash, _bool Loop)
{
	m_pPart = pPart;
	m_bDash = Dash;
	m_bLoop = Loop;
	m_iIndex = iIndex;
}

void CFail::Update(_float fTimedelta)
{
	m_pPart->Set_Animation("Fail", m_bLoop);
}

void CFail::Exit()
{
	m_pPart = nullptr;
	m_bDash = false;
	m_bLoop = false;
}

void CFail::Free()
{
	__super::Free();
}
