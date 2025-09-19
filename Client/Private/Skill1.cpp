#include "Skill1.h"
#include "PartObject.h"
CSkill1::CSkill1()
{
}

void CSkill1::Enter(CPartObject* pPart, _uint iIndex, _bool Dash, _bool Loop)
{
	m_pPart = pPart;
	m_bDash = Dash;
	m_bLoop = Loop;
	m_iIndex = iIndex;
}

void CSkill1::Update(_float fTimedelta)
{
	m_pPart->Set_Animation("Skill_1", m_bLoop);
}

void CSkill1::Exit()
{
	m_pPart = nullptr;
	m_bDash = false;
	m_bLoop = false;
}

void CSkill1::Free()
{
	__super::Free();
}
