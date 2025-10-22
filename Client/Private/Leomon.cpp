#include "Leomon.h"
#include "GameInstance.h"
#include "Body_Leomon.h"
#include "PartObject.h"
#include "StateMachine.h"
#include "Digimon_Manager.h"
#include "SkillObject.h"

CLeomon::CLeomon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CContainerObject{ pDevice, pContext }
{
}

CLeomon::CLeomon(const CLeomon& Prototype)
    : CContainerObject{ Prototype }
{
}

HRESULT CLeomon::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CLeomon::Initialize(void* pArg)
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

	__super::Set_Digimon_Info(m_pDigimon_Manager->Search_Digimon(4));

	return S_OK;
}

void CLeomon::Priority_Update(_float fTimeDelta)
{
	if (m_bLife)
	{
		__super::Priority_Update(fTimeDelta);
	}
}

void CLeomon::Update(_float fTimeDelta)
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
				m_iDamage = Info.DigimonSkill1Info.Damage / Info.DigimonSkill1Info.HitCount * 10000;
				if (static_cast<int>(m_pPart_Body->Get_TrackPosition()) == 17)
				{
					Creat_Skill(1);
				}
				if (static_cast<int>(m_pPart_Body->Get_TrackPosition()) == 31)
				{
					Creat_Skill(1);
				}
			}
			else if (m_bSkill2)
			{
				Skill2();
				m_iDamage = Info.DigimonSkill2Info.Damage / Info.DigimonSkill2Info.HitCount;
				if (static_cast<int>(m_pPart_Body->Get_TrackPosition()) == 42)
				{
					Creat_Skill(2);
				}
			}
			else if (m_bSkill3)
			{
				Skill3();
				m_iDamage = Info.DigimonSkill3Info.Damage / Info.DigimonSkill3Info.HitCount;
				if (static_cast<int>(m_pPart_Body->Get_TrackPosition()) == 30)
				{
					Creat_Skill(3);
				}
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

void CLeomon::Late_Update(_float fTimeDelta)
{
	if (m_bLife)
	{
		__super::Late_Update(fTimeDelta);

		m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
	}
}

HRESULT CLeomon::Render()
{
	if (m_bLife)
	{
#ifdef _DEBUG
		m_pColliderCom->Render();
#endif
	}
	return S_OK;
}

_int CLeomon::Intersect(CCollider* pPlayer_Collider)
{
	if (m_pColliderCom->Intersect(pPlayer_Collider))
		return Get_ID();

	return -1;
}

void CLeomon::UseSkill(_int Skill)
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

		m_bSkill2 = true;
	}

	else if (Skill == 3)
	{
		if (!m_bSkillMove)
			m_bSkillMove = true;
		m_bSkill3 = true;
	}
}

void CLeomon::Skill1()
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

void CLeomon::Skill2()
{
	m_pFsm->Enter(DIGIMONSTATE::SKILL2, m_pPart_Body, false, false);
	m_bSkill = true;
	if (m_bSkill2 && m_pPart_Body->Get_AnimFinish())
	{
		m_bSkill = false;
		m_bSkill2 = false;
	}
}

void CLeomon::Skill3()
{
	if (m_bSkillMove)
	{
		m_pFsm->Enter(DIGIMONSTATE::BATTLEDASH, m_pPart_Body);
	}
	else if (!m_bSkillMove)
	{
		m_pFsm->Enter(DIGIMONSTATE::SKILL3, m_pPart_Body, false, false);
		if (m_bSkill3 && m_pPart_Body->Get_AnimFinish())
		{
			m_bSkill = false;
			m_bSkill3 = false;
			m_bBackJump = true;
		}
	}
}

void CLeomon::Creat_Skill(_int SkillNum)
{
	CSkillObject::POSITION Desc;
	Desc.m_vPosition = m_pTransformCom->Get_State(STATE::POSITION);
	Desc.m_vTargetPosition = m_vTarget_Position;
	switch (SkillNum)
	{
	case 1:
		Desc.iDamage = m_iDamage;
		m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_LeomonSkill1"),
			ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_LeomonSkill1"), &Desc);
		break;

	case 2:
		Desc.iDamage = m_iDamage;
		m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_LeomonSkill2"),
			ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_LeomonSkill2"), &Desc);

		break;

	case 3:
		Desc.iDamage = m_iDamage;
		m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_LeomonSkill3"),
			ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_LeomonSkill3"), &Desc);

		break;
	}
}

HRESULT CLeomon::Ready_PartObjects()
{
	CBody_Leomon::BODY_PLAYER_DESC BodyDesc{};

	BodyDesc.pParentTransform = m_pTransformCom;

	/* Part_Body */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Leomon"),
		TEXT("Part_Body_Leomon"), &BodyDesc)))
		return E_FAIL;

	m_pPart_Body = dynamic_cast<CBody_Leomon*>(Find_PartObject(TEXT("Part_Body_Leomon")));

	/* Com_Sphere*/
	CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};
	SphereDesc.fRadius = 11.f;
	SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius, 0.f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider_Sphere"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
		return E_FAIL;

	return S_OK;
}

CLeomon* CLeomon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLeomon* pInstance = new CLeomon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLeomon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLeomon::Clone(void* pArg)
{
	CLeomon* pInstance = new CLeomon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CLeomon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLeomon::Free()
{
	__super::Free();

	Safe_Release(m_pFsm);
	Safe_Release(m_pColliderCom);

}
