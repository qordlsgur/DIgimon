#include "Ladydevimon.h"
#include "GameInstance.h"
#include "Body_Ladydevimon.h"
#include "PartObject.h"
#include "StateMachine.h"
#include "Digimon_Manager.h"

CLadydevimon::CLadydevimon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CContainerObject{ pDevice, pContext }
{
}

CLadydevimon::CLadydevimon(const CLadydevimon& Prototype)
    : CContainerObject{ Prototype }
{
}

HRESULT CLadydevimon::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CLadydevimon::Initialize(void* pArg)
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

	Digimon_Info Info;

	Info.DigimonName = TEXT("레이디데블몬");
	Info.DigimonId = 3;
	Info.Stage = DIGIMON_STAGE::ULTIMATE;
	Info.Attribute = DIGIMON_ATTRIBUTE::VIRUS;
	Info.DigimonInfo = TEXT("고귀한 존재인 여성형 타천사 디지몬");
	Info.Hp = 5000;
	Info.Sp = 100;
	Info.Damage = 500;
	Info.AttackSpeed = 100;
	Info.Exp = 0;
	Info.Lv = 50;

	__super::Set_Digimon_Info(Info);
	m_pDigimon_Manager->Digimon_Add(Info.DigimonId, Info);

	return S_OK;
}

void CLadydevimon::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CLadydevimon::Update(_float fTimeDelta)
{
	if (!m_bBattle)
	{
		//m_pTransformCom->LookAtPlayer(m_pDigimon_Manager->PlayerPos(), fTimeDelta);
		//m_bMove = false;
		//if (m_pTransformCom->FollowPlayer(m_pDigimon_Manager->PlayerPos(), 30, fTimeDelta))
		//{
		//	m_pFsm->Enter(DIGIMONSTATE::RUN, m_pPart_Body);
		//	m_bMove = true;
		//}

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
			m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(0));
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

void CLadydevimon::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CLadydevimon::Render()
{
#ifdef _DEBUG
	m_pColliderCom->Render();
#endif
	return S_OK;
}

void CLadydevimon::Skill1()
{
	m_pFsm->Enter(DIGIMONSTATE::SKILL1, m_pPart_Body, false, false);
}

void CLadydevimon::Skill2()
{
	m_pFsm->Enter(DIGIMONSTATE::SKILL2, m_pPart_Body, false, false);
}

void CLadydevimon::Skill3()
{
	m_pFsm->Enter(DIGIMONSTATE::SKILL3, m_pPart_Body, false, false);
}

HRESULT CLadydevimon::Ready_PartObjects()
{
	CBody_Ladydevimon::BODY_PLAYER_DESC BodyDesc{};

	BodyDesc.pParentTransform = m_pTransformCom;

	/* Part_Body */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Ladydevimon"),
		TEXT("Part_Body_Ladydevimon"), &BodyDesc)))
		return E_FAIL;

	m_pPart_Body = dynamic_cast<CBody_Ladydevimon*>(Find_PartObject(TEXT("Part_Body_Ladydevimon")));

	/* Com_Sphere*/
	CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};
	SphereDesc.fRadius = 11.f;
	SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius, 0.f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider_Sphere"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
		return E_FAIL;

	return S_OK;
}

CLadydevimon* CLadydevimon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLadydevimon* pInstance = new CLadydevimon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLadydevimon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLadydevimon::Clone(void* pArg)
{
	CLadydevimon* pInstance = new CLadydevimon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CLadydevimon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLadydevimon::Free()
{
	__super::Free();

	Safe_Release(m_pFsm);
	Safe_Release(m_pPart_Body);
	Safe_Release(m_pColliderCom);

}
