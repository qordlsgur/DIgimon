#include "Angewomon.h"
#include "GameInstance.h"
#include "Body_Angewomon.h"
#include "PartObject.h"
#include "StateMachine.h"
#include "Digimon_Manager.h"


CAngewomon::CAngewomon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CContainerObject{ pDevice, pContext }
{
}

CAngewomon::CAngewomon(const CAngewomon& Prototype)
	: CContainerObject{ Prototype }
{
}

HRESULT CAngewomon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAngewomon::Initialize(void* pArg)
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



	__super::Set_Digimon_Info(m_pDigimon_Manager->Search_Digimon(0));

	return S_OK;
}

void CAngewomon::Priority_Update(_float fTimeDelta)
{
	if (m_bLife)
	{
		__super::Priority_Update(fTimeDelta);
	}
}

void CAngewomon::Update(_float fTimeDelta)
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
			if (!Skill)
			{
				if (m_bSkill1)
				{
					Skill1();
				}
				else if (m_bSkill2)
				{
					Skill2();
				}
				else if (m_bSkill3)
				{
					Skill3();
				}
			}

			if (m_pPart_Body->Get_AnimFinish())
			{
				Skill = false;
				m_bTurnEnd = true;
			}

			if (m_bTurnEnd)
			{
				m_bTurnEnd = false;
				m_pFsm->Enter(DIGIMONSTATE::STANDBATTLE, m_pPart_Body);

			}
		}
		m_pFsm->Update(fTimeDelta);
		m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));

		__super::Update(fTimeDelta);
	}
}

void CAngewomon::Late_Update(_float fTimeDelta)
{
	if (m_bLife)
	{
		__super::Late_Update(fTimeDelta);

		m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
	}
}

HRESULT CAngewomon::Render()
{
	if (m_bLife)
	{
#ifdef _DEBUG
		m_pColliderCom->Render();
#endif
	}
	return S_OK;
}

void CAngewomon::UseSkill(_int Skill)
{
	if (Skill == 1)
	{
		m_bSkillMove = true;
		m_bSkill1 = true;
	}
	else if (Skill == 2)
		m_bSkill2 = true;
	else if (Skill == 3)
		m_bSkill3 = true;
}
void CAngewomon::Skill1()
{
	if (!m_bSkillMove)
		m_pFsm->Enter(DIGIMONSTATE::SKILL1, m_pPart_Body, false, false);
	Skill = true;
}

void CAngewomon::Skill2()
{
	m_pFsm->Enter(DIGIMONSTATE::SKILL2, m_pPart_Body, false, false);
	Skill = true;
}

void CAngewomon::Skill3()
{
	m_pFsm->Enter(DIGIMONSTATE::SKILL3, m_pPart_Body, false, false);
	Skill = true;
}

HRESULT CAngewomon::Ready_PartObjects()
{
	CBody_Angewomon::BODY_PLAYER_DESC BodyDesc{};

	BodyDesc.pParentTransform = m_pTransformCom;

	/* Part_Body */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Angewomon"),
		TEXT("Part_Body_Angewomon"), &BodyDesc)))
		return E_FAIL;

	m_pPart_Body = dynamic_cast<CBody_Angewomon*>(Find_PartObject(TEXT("Part_Body_Angewomon")));

	/* Com_Sphere*/
	CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};
	SphereDesc.fRadius = 10.f;
	SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius, 0.f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider_Sphere"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
		return E_FAIL;

	return S_OK;
}


CAngewomon* CAngewomon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CAngewomon* pInstance = new CAngewomon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CAngewomon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CAngewomon::Clone(void* pArg)
{
	CAngewomon* pInstance = new CAngewomon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CAngewomon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAngewomon::Free()
{
	__super::Free();

	Safe_Release(m_pFsm);
	Safe_Release(m_pPart_Body);
	Safe_Release(m_pColliderCom);
}
