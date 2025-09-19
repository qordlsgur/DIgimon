#include "Skill3.h"
#include "PartObject.h"

CSkill3::CSkill3()
{
}

void CSkill3::Enter(CPartObject* pPart, _uint iIndex, _bool Dash, _bool Loop)
{
	m_pPart = pPart;
	m_bDash = Dash;
	m_bLoop = Loop;
	m_iIndex = iIndex;
}

void CSkill3::Update(_float fTimedelta)
{
	m_pPart->Set_Animation("Skill_3", m_bLoop);
}

void CSkill3::Exit()
{
	m_pPart = nullptr;
	m_bDash = false;
	m_bLoop = false;
}

void CSkill3::Free()
{
	__super::Free();
}
