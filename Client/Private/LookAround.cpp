#include "LookAround.h"
#include "PartObject.h"

CLookAround::CLookAround()
{
}

void CLookAround::Enter(CPartObject* pPart, _uint iIndex, _bool Dash, _bool Loop)
{
	m_pPart = pPart;
	m_bDash = Dash;
	m_bLoop = Loop;
	m_iIndex = iIndex;
}

void CLookAround::Update(_float fTimedelta)
{
	m_pPart->Set_Animation("LookAround", m_bLoop);
}

void CLookAround::Exit()
{
	m_pPart = nullptr;
	m_bDash = false;
	m_bLoop = false;
}

void CLookAround::Free()
{
	__super::Free();
}
