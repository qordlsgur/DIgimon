#include "Omegamon.h"
#include "GameInstance.h"
#include "Body_Omegamon.h"
#include "PartObject.h"
#include "StateMachine.h"
#include "Digimon_Manager.h"
#include "SkillObject.h"

COmegamon::COmegamon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CContainerObject{ pDevice, pContext }
{
}

COmegamon::COmegamon(const COmegamon& Prototype)
	: CContainerObject{ Prototype }
{
}

HRESULT COmegamon::Initialize_Prototype()
{
	return S_OK;

}

HRESULT COmegamon::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC	Desc{};
	Desc.fRotationPerSec = XMConvertToRadians(180.0f);
	Desc.fSpeedPerSec = 70.f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	m_pDigimon_Manager = CDigimon_Manager::GetInstance();

	m_pFsm = CStateMachine::Create();
	m_pFsm->Initialize();
	m_pFsm->Enter(DIGIMONSTATE::STAND, m_pPart_Body, false, false);
	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(10.f, 0.f, 190.f, 1.f));

	__super::Set_Digimon_Info(m_pDigimon_Manager->Search_Digimon(7));

	return S_OK;
}

void COmegamon::Priority_Update(_float fTimeDelta)
{
	if (m_bLife)
	{
		__super::Priority_Update(fTimeDelta);
	}
}

void COmegamon::Update(_float fTimeDelta)
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

			if (m_pGameInstance->Key_Down(DIK_3))
			{
				static_cast<CBody_Omegamon*>(m_pPart_Body)->Set_Dissolve(true);
			}

			if (m_pGameInstance->Key_Down(DIK_2))
			{
				static_cast<CBody_Omegamon*>(m_pPart_Body)->Time();
			}

			if (m_pGameInstance->Key_Down(DIK_K))
			{
				static_cast<CBody_Omegamon*>(m_pPart_Body)->Num(-1);
			}

			if (m_pGameInstance->Key_Down(DIK_L))
			{
				static_cast<CBody_Omegamon*>(m_pPart_Body)->Num(+1);
			}
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

					m_iDamage = static_cast<_int>(Info.Damage * Info.DigimonSkill1Info.HitCount * 0.2f);
					m_iSkill = static_cast<_int>(m_pPart_Body->Get_TrackPosition());
					switch (m_iSkill)
					{
					case 10:
					case 22:
					case 42:
						if (m_iSkill != m_iLastSkill) // 이전과 다를 때만 실행
						{
							Creat_Skill(1);
							m_iLastSkill = m_iSkill;
						}
						break;
					default:
						m_iLastSkill = -1;
						break;
					}
				}
				else if (m_bSkill2)
				{
					Skill2();
					m_iDamage = static_cast<_int>(Info.Damage * Info.DigimonSkill2Info.HitCount * 1.f);
					if (static_cast<int>(m_pPart_Body->Get_TrackPosition()) == 67)
					{
						Creat_Skill(2);
					}
				}
				else if (m_bSkill3)
				{
					Skill3();

					m_iDamage = static_cast<_int>(Info.Damage * Info.DigimonSkill1Info.HitCount * 0.4f);
					m_iSkill = static_cast<_int>(m_pPart_Body->Get_TrackPosition());
					switch (m_iSkill)
					{
					case 70:
					case 80:
					case 90:
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

				if (!m_bSkill1 && !m_bSkill2 && !m_bSkill3 && !m_bBackJump && !m_bHitAinm)
				{
					m_pFsm->Enter(DIGIMONSTATE::STANDBATTLE, m_pPart_Body);
					m_bTurnEnd = true;
				}
				if (m_bHitAinm == true)
				{
					if (m_pPart_Body->Get_AnimFinish() == true)
						m_bHitAinm = false;
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

void COmegamon::Late_Update(_float fTimeDelta)
{
	if (m_bLife)
	{
		__super::Late_Update(fTimeDelta);

		m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
	}
}

HRESULT COmegamon::Render()
{

	return S_OK;
}

_int COmegamon::Intersect(CCollider* pPlayer_Collider)
{
	if (m_pColliderCom->Intersect(pPlayer_Collider))
		return Get_ID();

	return -1;
}

void COmegamon::HitAnim()
{
	m_bHitAinm = true;
	m_pFsm->Enter(DIGIMONSTATE::HIT, m_pPart_Body, false, false);
}

void COmegamon::UseSkill(_int Skill)
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
		m_bSkill2 = true;

	else if (Skill == 3)
		m_bSkill3 = true;
}

void COmegamon::Skill1()
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

void COmegamon::Skill2()
{
	m_pFsm->Enter(DIGIMONSTATE::SKILL2, m_pPart_Body, false, false);
	if (m_bSkill2 && m_pPart_Body->Get_AnimFinish())
	{
		m_bSkill = false;
		m_bSkill2 = false;
	}
}

void COmegamon::Skill3()
{
	m_pFsm->Enter(DIGIMONSTATE::SKILL3, m_pPart_Body, false, false);
	if (m_bSkill3 && m_pPart_Body->Get_AnimFinish())
	{
		m_bSkill = false;
		m_bSkill3 = false;
	}
}

void COmegamon::Creat_Skill(_int SkillNum)
{
	CSkillObject::POSITION Desc;
	Desc.m_vPosition = m_pTransformCom->Get_State(STATE::POSITION);
	Desc.m_vTargetPosition = m_vTarget_Position;
	Desc.Look = m_bMonster;
	switch (SkillNum)
	{
	case 1:
		Desc.iDamage = m_iDamage;
		m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_OmegamonSkill1"),
			ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_OmegamonSkill1"), &Desc);
		break;

	case 2:
		Desc.iDamage = m_iDamage;
		m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_OmegamonSkill2"),
			ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_OmegamonSkill2"), &Desc);

		break;

	case 3:
		Desc.iDamage = m_iDamage;
		m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_OmegamonSkill3"),
			ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_OmegamonSkill3"), &Desc);

		break;
	}
}


HRESULT COmegamon::Ready_PartObjects()
{
	CBody_Omegamon::BODY_PLAYER_DESC BodyDesc{};

	BodyDesc.pParentTransform = m_pTransformCom;

	/* Part_Body */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Omegamon"),
		TEXT("Part_Body_Omrgamon"), &BodyDesc)))
		return E_FAIL;

	m_pPart_Body = dynamic_cast<CBody_Omegamon*>(Find_PartObject(TEXT("Part_Body_Omrgamon")));

	/* Com_Sphere*/
	CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};
	SphereDesc.fRadius = 15.f;
	SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius, 0.f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider_Sphere"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
		return E_FAIL;

	return S_OK;
}

COmegamon* COmegamon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	COmegamon* pInstance = new COmegamon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : COmegamon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* COmegamon::Clone(void* pArg)
{
	COmegamon* pInstance = new COmegamon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : COmegamon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void COmegamon::Free()
{
	__super::Free();

	Safe_Release(m_pFsm);
	Safe_Release(m_pColliderCom);

}