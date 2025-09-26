#include "Omegamon.h"
#include "GameInstance.h"
#include "Body_Omegamon.h"
#include "PartObject.h"
#include "StateMachine.h"
#include "Digimon_Manager.h"

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
		}
		else
		{
			if (!Skill)
			{
				if (m_pGameInstance->Key_Down(DIK_4))
				{
					m_pFsm->Enter(DIGIMONSTATE::BATTLEBACK, m_pPart_Body, false, false);
					Skill = true;
				}

				if (m_pGameInstance->Key_Down(DIK_5))
				{
					m_pFsm->Enter(DIGIMONSTATE::BATTLEDASH, m_pPart_Body, false, false);
					Skill = true;
				}

				if (m_pGameInstance->Key_Down(DIK_6))
				{
					m_pFsm->Enter(DIGIMONSTATE::HIT, m_pPart_Body, false, false);
					Skill = true;
				}

				if (m_pGameInstance->Key_Down(DIK_7))
				{
					m_pFsm->Enter(DIGIMONSTATE::DEATH, m_pPart_Body, false, false);
					Skill = true;
				}

				if (m_pGameInstance->Key_Down(DIK_8))
				{
					m_pFsm->Enter(DIGIMONSTATE::FAIL, m_pPart_Body, false, false);
					Skill = true;
				}

				if (m_pGameInstance->Key_Down(DIK_9))
				{
					m_pFsm->Enter(DIGIMONSTATE::LOOKAROUND, m_pPart_Body, false, false);
					Skill = true;
				}
			}

			if (m_pPart_Body->Get_AnimFinish())
			{
				Skill = false;
			}

			if (!Skill)
			{
				m_bMove = false;
				//m_pFsm->Enter(DIGIMONSTATE::BATTLEDASH, m_pPart_Body);
				//m_bMove = true;
				if (!m_bMove)
					m_pFsm->Enter(DIGIMONSTATE::STANDBATTLE, m_pPart_Body);
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
	if (m_bLife)
	{
#ifdef _DEBUG
		m_pColliderCom->Render();
#endif
	}
	return S_OK;
}

_int COmegamon::Intersect(CCollider* pPlayer_Collider)
{
	if (m_pColliderCom->Intersect(pPlayer_Collider))
		return Get_ID();

	return -1;
}

void COmegamon::Skill1()
{
	m_pFsm->Enter(DIGIMONSTATE::SKILL1, m_pPart_Body, false, false);
}

void COmegamon::Skill2()
{
	m_pFsm->Enter(DIGIMONSTATE::SKILL2, m_pPart_Body, false, false);
}

void COmegamon::Skill3()
{
	m_pFsm->Enter(DIGIMONSTATE::SKILL3, m_pPart_Body, false, false);
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
	Safe_Release(m_pPart_Body);
	Safe_Release(m_pColliderCom);

}