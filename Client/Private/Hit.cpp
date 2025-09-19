#include "Hit.h"
#include "PartObject.h"

CHit::CHit()
{
}

void CHit::Enter(CPartObject* pPart, _uint iIndex, _bool Dash, _bool Loop)
{
	m_pPart = pPart;
	m_bDash = Dash;
	m_bLoop = Loop;
	m_iIndex = iIndex;
}

void CHit::Update(_float fTimedelta)
{
	m_pPart->Set_Animation("Hit1", m_bLoop);
}	

void CHit::Exit()
{
	m_pPart = nullptr;
	m_bDash = false;
	m_bLoop = false;
}

void CHit::Free()
{
	__super::Free();
}
