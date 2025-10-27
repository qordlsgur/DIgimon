#include "Interaction_Manager.h"
#include "ContainerObject.h"
#include "Collider.h"
#include "SkillObject.h"
#include "Player.h"

IMPLEMENT_SINGLETON(CInteraction_Manager);

CInteraction_Manager::CInteraction_Manager()
{
}

HRESULT CInteraction_Manager::Initialize()
{
	return S_OK;
}

void CInteraction_Manager::Priority_Update()
{
}

void CInteraction_Manager::Update()
{
	if (m_pAttackDigimon != nullptr)
	{
		for (auto it : m_pBattle_Digimon_Collider)
		{
			if (it->Intersect(m_pSkill_Collider) != -1)
			{
				if (!m_pAttackDigimon->Get_Hit())
				{
					it->Set_HitDamage(m_pAttackDigimon->Get_Damage());
					m_pAttackDigimon->Set_Hit(true);
				}
			}
		}
	}
	if (m_pAttackSkill != nullptr)
	{
		for (auto it : m_pBattle_Digimon_Collider)
		{
			if (it->Intersect(m_pSkill_Collider) != -1)
			{
				if (!m_pAttackDigimon->Get_Hit())
				{
					it->Set_HitDamage(m_pAttackDigimon->Get_Damage());
					m_pAttackDigimon->Set_Hit(true);
				}
			}
		}
	}
}

void CInteraction_Manager::Late_Update()
{
	for (_int i = 0; i < m_pEnemy_Digimon.size(); ++i)
	{
		if (m_pEnemy_Digimon[i]->Intersect(m_pPlayer_Collider) != -1)
		{
			_int j = m_pEnemy_Digimon[i]->Intersect(m_pPlayer_Collider);
			static_cast<CPlayer*>(m_pPlayer)->Intersect_Enemy(j);
		}
	}



}

void CInteraction_Manager::Set_Player(CContainerObject* pPlayer)
{
	if (nullptr == m_pPlayer)
		m_pPlayer = pPlayer;
}

void CInteraction_Manager::Set_Player_Collider(CCollider* pCollider)
{
	m_pPlayer_Collider = pCollider;
}

void CInteraction_Manager::Set_Enemy_Digimon(CContainerObject* Enemy)
{
	m_pEnemy_Digimon.push_back(Enemy);
}

void CInteraction_Manager::Set_Attack_Digimon(CSkillObject* Attacker)
{
	m_pAttackDigimon = Attacker;
}

void CInteraction_Manager::Set_Attack_Skill(CPartObject* Attacker)
{
	m_pAttackSkill = Attacker;
}

void CInteraction_Manager::Set_Hit_Digimon(CContainerObject* Battle_Digimon)
{
	m_pBattle_Digimon_Collider.push_back(Battle_Digimon);
}

void CInteraction_Manager::Set_Skill_Collider(CCollider* pCollider)
{
	m_pSkill_Collider = pCollider;
}

void CInteraction_Manager::Battle_End()
{
	m_pBattle_Digimon_Collider.clear();
}

void CInteraction_Manager::Free()
{
	__super::Free();

	m_pEnemy_Digimon.clear();
	m_pBattle_Digimon_Collider.clear();
}
