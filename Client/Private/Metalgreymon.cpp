#include "Metalgreymon.h"
#include "GameInstance.h"
#include "Body_Metalgreymon.h"
#include "PartObject.h"
#include "StateMachine.h"
#include "Digimon_Manager.h"
#include "SkillObject.h"

CMetalgreymon::CMetalgreymon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CContainerObject{ pDevice, pContext }
{
}

CMetalgreymon::CMetalgreymon(const CMetalgreymon& Prototype)
	: CContainerObject{ Prototype }
{
}

HRESULT CMetalgreymon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMetalgreymon::Initialize(void* pArg)
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

	__super::Set_Digimon_Info(m_pDigimon_Manager->Search_Digimon(6));

	return S_OK;
}

void CMetalgreymon::Priority_Update(_float fTimeDelta)
{
	if (m_bLife)
	{
		__super::Priority_Update(fTimeDelta);
	}
}

void CMetalgreymon::Update(_float fTimeDelta)
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
				m_bDie = true;
			}

			if (!m_bDie)
			{
				if (m_bSkill1)
				{
					Skill1();
					m_iDamage = static_cast<_int>(Info.Damage * Info.DigimonSkill2Info.HitCount * 0.5f);
					m_iSkill = static_cast<_int>(m_pPart_Body->Get_TrackPosition());

					if (m_iSkill == 14)
					{
						if (m_iLastSkill != m_iSkill)
						{
							Creat_Skill(1);
							m_iLastSkill = m_iSkill; // 마지막으로 실행한 트랙 위치 저장
						}
					}
					else
					{
						m_iLastSkill = -1; // 다른 트랙 위치면 초기화
					}
				}
				else if (m_bSkill2)
				{
					Skill2();
					m_iDamage = static_cast<_int>(Info.Damage * Info.DigimonSkill2Info.HitCount * 1.f);
					m_iSkill = static_cast<_int>(m_pPart_Body->Get_TrackPosition());

					if (m_iSkill == 23)
					{
						if (m_iLastSkill != m_iSkill)
						{
							Creat_Skill(2);
							m_iLastSkill = m_iSkill; // 마지막으로 실행한 트랙 위치 저장
						}
					}
					else
					{
						m_iLastSkill = -1; // 다른 트랙 위치면 초기화
					}
				}
				else if (m_bSkill3)
				{
					Skill3();
					m_iDamage = static_cast<_int>(Info.Damage * Info.DigimonSkill2Info.HitCount * 1.2f);
					m_iSkill = static_cast<_int>(m_pPart_Body->Get_TrackPosition());

					if (m_iSkill == 23)
					{
						if (m_iLastSkill != m_iSkill)
						{
							Creat_Skill(3);
							m_iLastSkill = m_iSkill; // 마지막으로 실행한 트랙 위치 저장
						}
					}
					else
					{
						m_iLastSkill = -1; // 다른 트랙 위치면 초기화
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

			else
			{
				m_pFsm->Enter(DIGIMONSTATE::DEATH, m_pPart_Body, false, false);

				if (m_pPart_Body->Get_AnimFinish())
				{
					if (m_bMonster)
					{
						m_pPart_Body->Set_Dissolve(true);
					}

				}

			}
		}
		m_pFsm->Update(fTimeDelta);
		m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));

		__super::Update(fTimeDelta);
	}
}

void CMetalgreymon::Late_Update(_float fTimeDelta)
{
	if (m_bLife)
	{
		__super::Late_Update(fTimeDelta);

		m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
	}
}

HRESULT CMetalgreymon::Render()
{
	if (m_bLife)
	{
#ifdef _DEBUG
		m_pColliderCom->Render();
#endif
	}
	return S_OK;
}

_int CMetalgreymon::Intersect(CCollider* pPlayer_Collider)
{
	if (m_pColliderCom->Intersect(pPlayer_Collider))
		return Get_ID();

	return -1;
}

void CMetalgreymon::UseSkill(_int Skill)
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

void CMetalgreymon::Skill1()
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

void CMetalgreymon::Skill2()
{
	m_pFsm->Enter(DIGIMONSTATE::SKILL2, m_pPart_Body, false, false);
	if (m_bSkill2 && m_pPart_Body->Get_AnimFinish())
	{
		m_bSkill = false;
		m_bSkill2 = false;
	}
}

void CMetalgreymon::Skill3()
{
	m_pFsm->Enter(DIGIMONSTATE::SKILL3, m_pPart_Body, false, false);
	m_bSkill = true;
	if (m_bSkill3 && m_pPart_Body->Get_AnimFinish())
	{
		m_bSkill = false;
		m_bSkill3 = false;
	}
}

void CMetalgreymon::Creat_Skill(_int SkillNum)
{
	CSkillObject::POSITION Desc;
	Desc.m_vPosition = m_pTransformCom->Get_State(STATE::POSITION);
	Desc.m_vTargetPosition = m_vTarget_Position;
	Desc.Look = m_bMonster;
	switch (SkillNum)
	{
	case 1:
		Desc.iDamage = m_iDamage;
		m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgreymonSkill1"),
			ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_MetalgreymonSkill1"), &Desc);
		break;

	case 2:
		Desc.iDamage = m_iDamage;
		m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgreymonSkill2"),
			ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_MetalgreymonSkill2"), &Desc);

		break;

	case 3:
		Desc.iDamage = m_iDamage;
		m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgreymonSkill3"),
			ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_MetalgreymonSkill3"), &Desc);

		break;
	}
}

HRESULT CMetalgreymon::Ready_PartObjects()
{
	CBody_Metalgreymon::BODY_PLAYER_DESC BodyDesc{};

	BodyDesc.pParentTransform = m_pTransformCom;

	/* Part_Body */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Metalgreymon"),
		TEXT("Part_Body_Metalgreymon"), &BodyDesc)))
		return E_FAIL;

	m_pPart_Body = dynamic_cast<CBody_Metalgreymon*>(Find_PartObject(TEXT("Part_Body_Metalgreymon")));

	/* Com_Sphere*/
	CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};
	SphereDesc.fRadius = 11.f;
	SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius, 0.f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider_Sphere"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
		return E_FAIL;

	return S_OK;
}

CMetalgreymon* CMetalgreymon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMetalgreymon* pInstance = new CMetalgreymon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMetalgreymon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMetalgreymon::Clone(void* pArg)
{
	CMetalgreymon* pInstance = new CMetalgreymon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMetalgreymon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMetalgreymon::Free()
{
	__super::Free();

	Safe_Release(m_pFsm);
	Safe_Release(m_pColliderCom);

}
