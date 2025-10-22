#include "Devilmon.h"
#include "GameInstance.h"
#include "Body_Devilmon.h"
#include "PartObject.h"
#include "StateMachine.h"
#include "Digimon_Manager.h"

CDevilmon::CDevilmon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CContainerObject{ pDevice, pContext }
{
}

CDevilmon::CDevilmon(const CDevilmon& Prototype)
    : CContainerObject{ Prototype }
{
}

HRESULT CDevilmon::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CDevilmon::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC	Desc{};
	Desc.fRotationPerSec = XMConvertToRadians(180.0f);
	Desc.fSpeedPerSec = 50.f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	m_pDigimon_Manager = CDigimon_Manager::GetInstance();

	m_pFsm = CStateMachine::Create();
	m_pFsm->Initialize();
	m_pFsm->Enter(DIGIMONSTATE::STAND, m_pPart_Body, false, false);
	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(10.f, 0.f, 190.f, 1.f));

	__super::Set_Digimon_Info(m_pDigimon_Manager->Search_Digimon(2));

	return S_OK;
}

void CDevilmon::Priority_Update(_float fTimeDelta)
{
	if (m_bLife)
	{
		__super::Priority_Update(fTimeDelta);
	}
}

void CDevilmon::Update(_float fTimeDelta)
{
	if (m_bLife)
	{
		if (!m_bBattle)
		{
			if (!m_bMonster)
			{
				m_pTransformCom->LookAtPlayer(m_pDigimon_Manager->PlayerPos(), fTimeDelta);
				m_bMove = false;
				if (m_pTransformCom->FollowPlayer(m_pDigimon_Manager->PlayerPos(), 30, fTimeDelta))
				{
					m_pFsm->Enter(DIGIMONSTATE::RUN, m_pPart_Body);
					m_bMove = true;
				}
			}
			if (!m_bMove)
				m_pFsm->Enter(DIGIMONSTATE::STAND, m_pPart_Body);
		}
		else
		{
			if (Info.Hp <= 0)
			{
				Info.Hp = 0;
				m_bLife = false;
			}

			if (m_bSkill1)
			{
				Skill1();
				m_iDamage = Info.DigimonSkill1Info.Damage;
			}
			else if (m_bSkill2)
			{
				Skill2();
				m_iDamage = Info.DigimonSkill2Info.Damage;

			}
			else if (m_bSkill3)
			{
				Skill3();
				m_iDamage = Info.DigimonSkill3Info.Damage;
			}

			if (m_bBackJump)
			{
				m_pFsm->Enter(DIGIMONSTATE::BATTLEBACK, m_pPart_Body);
			}

			if (!m_bSkill1 && !m_bSkill2 && !m_bSkill3 && !m_bBackJump)
			{
				m_bTurnEnd = true;
				m_pFsm->Enter(DIGIMONSTATE::STANDBATTLE, m_pPart_Body);
			}
		}
		m_pFsm->Update(fTimeDelta);
		m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));

		__super::Update(fTimeDelta);
	}
}

void CDevilmon::Late_Update(_float fTimeDelta)
{
	if (m_bLife)
	{
		__super::Late_Update(fTimeDelta);

		m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
	}
}

HRESULT CDevilmon::Render()
{
	if (m_bLife)
	{
#ifdef _DEBUG
		m_pColliderCom->Render();
#endif
	}
	return S_OK;
}

_int CDevilmon::Intersect(CCollider* pPlayer_Collider)
{
	if (m_pColliderCom->Intersect(pPlayer_Collider))
		return Get_ID();

	return -1;
}

void CDevilmon::UseSkill(_int Skill)
{
	m_bSkill = true;
	m_bTurnEnd = false;
	if (Skill == 1)
	{
		if (!m_bSkillMove)
			m_bSkillMove = true;
		m_bSkill1 = true;
	}

	else if (Skill == 2)
	{
		if (!m_bSkillMove)
			m_bSkillMove = true;
		m_bSkill2 = true;
	}

	else if (Skill == 3)
		m_bSkill3 = true;
}

void CDevilmon::Skill1()
{
	if (m_bSkillMove)
	{
		m_pFsm->Enter(DIGIMONSTATE::BATTLEDASH, m_pPart_Body);
	}
	else if (!m_bSkillMove)
	{
		m_pFsm->Enter(DIGIMONSTATE::SKILL1, m_pPart_Body, false, false);
		if (m_bSkill1 && m_pPart_Body->Get_AnimFinish())
		{
			m_bSkill = false;
			m_bSkill1 = false;
			m_bBackJump = true;
		}
	}
}

void CDevilmon::Skill2()
{
	if (m_bSkillMove)
	{
		m_pFsm->Enter(DIGIMONSTATE::BATTLEDASH, m_pPart_Body);
	}
	else if (!m_bSkillMove)
	{
		m_pFsm->Enter(DIGIMONSTATE::SKILL2, m_pPart_Body, false, false);
		if (m_bSkill2 && m_pPart_Body->Get_AnimFinish())
		{
			m_bSkill = false;
			m_bSkill2 = false;
			m_bBackJump = true;
		}
	}
}

void CDevilmon::Skill3()
{
	m_pFsm->Enter(DIGIMONSTATE::SKILL3, m_pPart_Body, false, false);
	m_bSkill = true;
	if (m_bSkill3 && m_pPart_Body->Get_AnimFinish())
	{
		m_bSkill = false;
		m_bSkill3 = false;
	}
}

HRESULT CDevilmon::Ready_PartObjects()
{
	CBody_Devilmon::BODY_PLAYER_DESC BodyDesc{};

	BodyDesc.pParentTransform = m_pTransformCom;

	/* Part_Body */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Devilmon"),
		TEXT("Part_Body_Devilmon"), &BodyDesc)))
		return E_FAIL;

	m_pPart_Body = dynamic_cast<CBody_Devilmon*>(Find_PartObject(TEXT("Part_Body_Devilmon")));

	/* Com_Sphere*/
	CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};
	SphereDesc.fRadius = 8.f;
	SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius, 0.f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider_Sphere"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
		return E_FAIL;

	return S_OK;
}

CDevilmon* CDevilmon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDevilmon* pInstance = new CDevilmon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDevilmon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDevilmon::Clone(void* pArg)
{
	CDevilmon* pInstance = new CDevilmon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDevilmon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDevilmon::Free()
{
	__super::Free();

	Safe_Release(m_pFsm);
	Safe_Release(m_pColliderCom);

}

