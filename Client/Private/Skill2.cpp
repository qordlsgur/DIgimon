#include "Skill2.h"
#include "PartObject.h"
CSkill2::CSkill2()
{
}

void CSkill2::Enter(CPartObject* pPart, _uint iIndex, _bool Dash, _bool Loop)
{
	m_pPart = pPart;
	m_bDash = Dash;
	m_bLoop = Loop;
	m_iIndex = iIndex;
}

void CSkill2::Update(_float fTimedelta)
{
	m_pPart->Set_Animation("Skill_2", m_bLoop);
}

void CSkill2::Exit()
{
	m_pPart = nullptr;
	m_bDash = false;
	m_bLoop = false;
}

void CSkill2::Free()
{
	__super::Free();
}
