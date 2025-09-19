#include "Run.h"
#include "PartObject.h"

CRun::CRun()
{
}

void CRun::Enter(CPartObject* pPart, _uint iIndex, _bool Dash, _bool Loop)
{
	m_pPart = pPart;
	m_bDash = Dash;
	m_bLoop = Loop;
	m_iIndex = iIndex;
}

void CRun::Update(_float fTimedelta)
{
	m_pPart->Set_Animation("Run", m_bLoop);
}

void CRun::Exit()
{
	m_pPart = nullptr;
	m_bDash = false;
	m_bLoop = false;
}

void CRun::Free()
{
	__super::Free();
}
