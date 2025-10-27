#include "Blackwargreymon.h"
#include "GameInstance.h"
#include "Body_Blackwargreymon.h"
#include "PartObject.h"
#include "StateMachine.h"
#include "Digimon_Manager.h"
#include "SkillObject.h"
#include "BlackwargreymonSkill3.h"
#include "SkillObject.h"
#include "SkillObject.h"

CBlackwargreymon::CBlackwargreymon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CContainerObject{ pDevice, pContext }
{
}

CBlackwargreymon::CBlackwargreymon(const CBlackwargreymon& Prototype)
	: CContainerObject{ Prototype }
{
}

HRESULT CBlackwargreymon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBlackwargreymon::Initialize(void* pArg)
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

	__super::Set_Digimon_Info(m_pDigimon_Manager->Search_Digimon(1));


	return S_OK;
}

void CBlackwargreymon::Priority_Update(_float fTimeDelta)
{
	if (m_bLife)
	{
		__super::Priority_Update(fTimeDelta);
	}
}

void CBlackwargreymon::Update(_float fTimeDelta)
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
					m_iDamage = static_cast<_int>(Info.Damage * Info.DigimonSkill1Info.HitCount * 0.4f);
					m_iSkill = static_cast<_int>(m_pPart_Body->Get_TrackPosition());
					switch (m_iSkill)
					{
					case 23:
					case 38:
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
					m_iSkill = static_cast<_int>(m_pPart_Body->Get_TrackPosition());
					if (m_iSkill == 54)
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
					switch (m_iSkill)
					{
					case 13:
						if (m_iSkill != m_iLastSkill)
						{
							Creat_Skill(3);
							m_iLastSkill = m_iSkill;
						}
						break;
					case 109:
					case 110:
					case 111:
						if (m_iSkill != m_iLastSkill)
						{
							m_pSkill1->Set_Move(true);
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
//if (m_pGameInstance->Key_Down(DIK_1))
//{
//	m_bisHit = !m_bisHit;
//	m_fRandom = m_pGameInstance->Random(-3.f, 3.f);
//}

//if (m_bisHit)
//{
//	DamageUp(fTimeDelta);
//}
//else
//{
//	m_fTime = 0.f;
//	m_fFontUp = 13.5f;
//}
void CBlackwargreymon::Late_Update(_float fTimeDelta)
{
	if (m_bLife)
	{
		__super::Late_Update(fTimeDelta);

		m_pGameInstance->Add_RenderGroup(RENDER::UI, this);
	}
}

HRESULT CBlackwargreymon::Render()
{
	if (m_bLife)
	{
		if (m_bisHit)
		{
			m_pGameInstance->Perspective_Render_Text(
				m_pGameInstance->Get_Transform_Matrix(D3DTS::VIEW),
				m_pGameInstance->Get_Transform_Matrix(D3DTS::PROJ),
				TEXT("42"), TEXT("111"),
				XMVectorSet(m_pTransformCom->Get_State(STATE::POSITION).m128_f32[0] + m_fRandom,
					m_pTransformCom->Get_State(STATE::POSITION).m128_f32[1] + m_fFontUp,
					m_pTransformCom->Get_State(STATE::POSITION).m128_f32[2] + 5.f,
					m_pTransformCom->Get_State(STATE::POSITION).m128_f32[3]));
		}

#ifdef _DEBUG
		m_pColliderCom->Render();
#endif

	}
		return S_OK;
}

_int CBlackwargreymon::Intersect(CCollider* pPlayer_Collider)
{
	if (m_pColliderCom->Intersect(pPlayer_Collider))
		return Get_ID();

	return -1;
}

//void CBlackwargreymon::DamageUp(_float fTimeDelta)
//{
//	//m_fTime += fTimeDelta;
//	m_fFontUp += 0.8f;
//
//	if (m_fFontUp >= 27.f)
//		m_bisHit = false;
//}

void CBlackwargreymon::UseSkill(_int Skill)
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
	{

		m_bSkill3 = true;
	}


}

void CBlackwargreymon::Skill1()
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

void CBlackwargreymon::Skill2()
{
	m_pFsm->Enter(DIGIMONSTATE::SKILL2, m_pPart_Body, false, false);
	if (m_bSkill2 && m_pPart_Body->Get_AnimFinish())
	{
		m_bSkill = false;
		m_bSkill2 = false;
	}
}

void CBlackwargreymon::Skill3()
{
	m_pFsm->Enter(DIGIMONSTATE::SKILL3, m_pPart_Body, false, false);
	m_bSkill = true;
	if (m_bSkill3 && m_pPart_Body->Get_AnimFinish())
	{
		m_bSkill = false;
		m_bSkill3 = false;
	}
}

void CBlackwargreymon::Creat_Skill(_int SkillNum)
{
	CSkillObject::POSITION Desc;
	Desc.m_vPosition = m_pTransformCom->Get_State(STATE::POSITION);
	Desc.m_vTargetPosition = m_vTarget_Position;
	Desc.Look = m_bMonster;
	switch (SkillNum)
	{
	case 1:
		Desc.iDamage = m_iDamage;
		m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_BlackwargreymonSkill1"),
			ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_BlackwargreymonSkill1"), &Desc);
		break;

	case 2:
		Desc.iDamage = m_iDamage;
		m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_BlackwargreymonSkill2"),
			ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_BlackwargreymonSkill1"), &Desc);

		break;

	case 3:
		Desc.iDamage = m_iDamage;
		m_pSkill1 = static_cast<CBlackwargreymonSkill3*>(m_pGameInstance->Add_GameObject_ToLayer_ToCreate(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_BlackwargreymonSkill3"),
			ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_BlackwargreymonSkill3"), &Desc));

		break;
	}
}


HRESULT CBlackwargreymon::Ready_PartObjects()
{
	CBody_Blackwargreymon::BODY_PLAYER_DESC BodyDesc{};

	BodyDesc.pParentTransform = m_pTransformCom;

	/* Part_Body */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Blackwargreymon"),
		TEXT("Part_Body_Blackwargreymon"), &BodyDesc)))
		return E_FAIL;

	m_pPart_Body = dynamic_cast<CBody_Blackwargreymon*>(Find_PartObject(TEXT("Part_Body_Blackwargreymon")));

	/* Com_Sphere*/
	CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};
	SphereDesc.fRadius = 12.f;
	SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius, 0.f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider_Sphere"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
		return E_FAIL;


	return S_OK;
}

CBlackwargreymon* CBlackwargreymon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBlackwargreymon* pInstance = new CBlackwargreymon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBlackwargreymon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBlackwargreymon::Clone(void* pArg)
{
	CBlackwargreymon* pInstance = new CBlackwargreymon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBlackwargreymon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBlackwargreymon::Free()
{
	__super::Free();

	Safe_Release(m_pFsm);
	Safe_Release(m_pColliderCom);

}
