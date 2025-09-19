#include "Death.h"
#include "PartObject.h"

CDeath::CDeath()
{
}

void CDeath::Enter(CPartObject* pPart, _uint iIndex, _bool Dash, _bool Loop)
{
	m_pPart = pPart;
	m_bDash = Dash;
	m_bLoop = Loop;
	m_iIndex = iIndex;
}

void CDeath::Update(_float fTimedelta)
{
	m_pPart->Set_Animation("Death", m_bLoop);
}

void CDeath::Exit()
{
	m_pPart = nullptr;
	m_bDash = false;
	m_bLoop = false;
}

void CDeath::Free()
{
	__super::Free();
}
