#include "Jump.h"
#include "PartObject.h"

CJump::CJump()
{
}

void CJump::Enter(CPartObject* pPart, _uint iIndex, _bool Dash, _bool Loop)
{
	m_pPart = pPart;
	m_bDash = Dash;
	m_bLoop = Loop;
	m_iIndex = iIndex;
}

void CJump::Update(_float fTimedelta)
{
	m_pPart->Set_Animation("Jump", m_bLoop);
}

void CJump::Exit()
{
	m_pPart = nullptr;
	m_bDash = false;
	m_bLoop = false;
}

void CJump::Free()
{
	__super::Free();
}
