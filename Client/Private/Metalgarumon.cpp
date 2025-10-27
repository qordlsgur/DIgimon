#include "Metalgarumon.h"
#include "GameInstance.h"
#include "Body_Metalgarumon.h"
#include "PartObject.h"
#include "StateMachine.h"
#include "Digimon_Manager.h"
#include "SkillObject.h"
#include "MetalgarumonSkill3.h"

CMetalgarumon::CMetalgarumon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CContainerObject{ pDevice, pContext }
{
}

CMetalgarumon::CMetalgarumon(const CMetalgarumon& Prototype)
	: CContainerObject{ Prototype }
{
}

HRESULT CMetalgarumon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMetalgarumon::Initialize(void* pArg)
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

	__super::Set_Digimon_Info(m_pDigimon_Manager->Search_Digimon(5));

	return S_OK;
}

void CMetalgarumon::Priority_Update(_float fTimeDelta)
{
	if (m_bLife)
	{
		__super::Priority_Update(fTimeDelta);
	}
}

void CMetalgarumon::Update(_float fTimeDelta)
{
	if (m_bLife)
	{
		if (!m_bBattle)
		{
			//if (!m_bMonster)
			//{
			//	m_pTransformCom->LookAtPlayer(m_pDigimon_Manager->PlayerPos(), fTimeDelta);
			//	m_bMove = false;
			//	if (m_pTransformCom->FollowPlayer(m_pDigimon_Manager->PlayerPos(), 30, fTimeDelta))
			//	{
			//		m_pFsm->Enter(DIGIMONSTATE::RUN, m_pPart_Body);
			//		m_bMove = true;
			//	}
			//}
			//if (!m_bMove)
			//	m_pFsm->Enter(DIGIMONSTATE::STAND, m_pPart_Body);
			if (m_pGameInstance->Key_Down(DIK_1))
			{
				m_pFsm->Enter(DIGIMONSTATE::SKILL3, m_pPart_Body, false, false);
				m_bMove = true;
			}
			m_iDamage = static_cast<_int>(Info.Damage * Info.DigimonSkill1Info.HitCount * 0.4f);
			m_iSkill = static_cast<_int>(m_pPart_Body->Get_TrackPosition());
			switch (m_iSkill)
			{
			case 42:
				if (m_iSkill != m_iLastSkill) // 이전과 다를 때만 실행
				{
					Creat_Skill(3);
					m_iLastSkill = m_iSkill;
				}
				break;
			case 56:
			case 70:
				m_pSkill->Set_Hit(false);
			default:
				m_iLastSkill = -1;
				break;
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

					if (m_iSkill == 21)
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

					if (m_iSkill == 48)
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

					m_iDamage = static_cast<_int>(Info.Damage * Info.DigimonSkill1Info.HitCount * 0.4f);
					m_iSkill = static_cast<_int>(m_pPart_Body->Get_TrackPosition());
					switch (m_iSkill)
					{
					case 42:
					case 56:
					case 70:
						if (m_iSkill != m_iLastSkill) // 이전과 다를 때만 실행
						{
							Creat_Skill(3);
							m_iLastSkill = m_iSkill;
						}
						break;
					default:
						m_iLastSkill = -1;
						break;
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
				m_pFsm->Enter(DIGIMONSTATE::DEATH, m_pPart_Body);

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

void CMetalgarumon::Late_Update(_float fTimeDelta)
{
	if (m_bLife)
	{
		__super::Late_Update(fTimeDelta);

		m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
	}
}

HRESULT CMetalgarumon::Render()
{
	if (m_bLife)
	{
#ifdef _DEBUG
		m_pColliderCom->Render();
#endif
	}
	return S_OK;
}

_int CMetalgarumon::Intersect(CCollider* pPlayer_Collider)
{
	if (m_pColliderCom->Intersect(pPlayer_Collider))
		return Get_ID();

	return -1;
}

void CMetalgarumon::UseSkill(_int Skill)
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
		m_bSkill3 = true;
}

void CMetalgarumon::Skill1()
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

void CMetalgarumon::Skill2()
{

	m_pFsm->Enter(DIGIMONSTATE::SKILL2, m_pPart_Body, false, false);
	if (m_bSkill2 && m_pPart_Body->Get_AnimFinish())
	{
		m_bSkill = false;
		m_bSkill2 = false;
	}
}

void CMetalgarumon::Skill3()
{
	m_pFsm->Enter(DIGIMONSTATE::SKILL3, m_pPart_Body, false, false);
	m_bSkill = true;
	if (m_bSkill3 && m_pPart_Body->Get_AnimFinish())
	{
		m_bSkill = false;
		m_bSkill3 = false;
	}
}

void CMetalgarumon::Creat_Skill(_int SkillNum)
{
	CSkillObject::POSITION Desc;
	Desc.m_vPosition = m_pTransformCom->Get_State(STATE::POSITION);
	Desc.m_vTargetPosition = m_vTarget_Position;
	Desc.Look = m_bMonster;
	switch (SkillNum)
	{
	case 1:
		Desc.iDamage = m_iDamage;
		m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgarumonSkill1"),
			ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_MetalgarumonSkill1"), &Desc);
		break;

	case 2:
		Desc.iDamage = m_iDamage;
		m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgarumonSkill2"),
			ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_MetalgarumonSkill2"), &Desc);

		break;

	case 3:
		Desc.iDamage = m_iDamage;
		m_pSkill = static_cast<CMetalgarumonSkill3*>(m_pGameInstance->Add_GameObject_ToLayer_ToCreate(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgarumonSkill3"),
			ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_MetalgarumonSkill3"), &Desc));

		break;
	}
}

HRESULT CMetalgarumon::Ready_PartObjects()
{
	CBody_Metalgarumon::BODY_PLAYER_DESC BodyDesc{};

	BodyDesc.pParentTransform = m_pTransformCom;

	/* Part_Body */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Metalgarumon"),
		TEXT("Part_Body_Metalgarumon"), &BodyDesc)))
		return E_FAIL;

	m_pPart_Body = dynamic_cast<CBody_Metalgarumon*>(Find_PartObject(TEXT("Part_Body_Metalgarumon")));

	/* Com_Sphere*/
	CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};
	SphereDesc.fRadius = 8.f;
	SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius, 0.f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider_Sphere"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
		return E_FAIL;

	return S_OK;
}

CMetalgarumon* CMetalgarumon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMetalgarumon* pInstance = new CMetalgarumon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMetalgarumon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMetalgarumon::Clone(void* pArg)
{
	CMetalgarumon* pInstance = new CMetalgarumon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMetalgarumon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMetalgarumon::Free()
{
	__super::Free();

	Safe_Release(m_pFsm);
	Safe_Release(m_pColliderCom);

}
