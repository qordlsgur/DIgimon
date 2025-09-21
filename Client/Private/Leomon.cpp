#include "Leomon.h"
#include "GameInstance.h"
#include "Body_Leomon.h"
#include "PartObject.h"
#include "StateMachine.h"
#include "Digimon_Manager.h"

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

	Digimon_Info Info;

	Info.DigimonName = TEXT("레오몬");
	Info.DigimonId = 0;
	Info.Stage = DIGIMON_STAGE::CHAMPION;
	Info.Attribute = DIGIMON_ATTRIBUTE::VACCINE;
	Info.DigimonInfo = TEXT("백수의 왕, 고상한 용사라 불리는 수인형 디지몬");
	Info.Hp = 2500;
	Info.Sp = 100;
	Info.Damage = 300;
	Info.AttackSpeed = 70;
	Info.Exp = 0;
	Info.Lv = 15;

	__super::Set_Digimon_Info(Info);

	return S_OK;
}

void CLeomon::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CLeomon::Update(_float fTimeDelta)
{
	if (!Skill)
	{
		if (m_pGameInstance->Key_Down(DIK_1))
		{
			m_pFsm->Enter(DIGIMONSTATE::SKILL1, m_pPart_Body, false, false);
			Skill = true;
		}

		if (m_pGameInstance->Key_Down(DIK_2))
		{
			m_pFsm->Enter(DIGIMONSTATE::SKILL2, m_pPart_Body, false, false);
			Skill = true;
		}

		if (m_pGameInstance->Key_Down(DIK_3))
		{
			m_pFsm->Enter(DIGIMONSTATE::SKILL3, m_pPart_Body, false, false);
			Skill = true;
		}
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
		m_pTransformCom->LookAtPlayer(m_pDigimon_Manager->PlayerPos(), fTimeDelta);
		m_bMove = false;
		if (m_pTransformCom->FollowPlayer(m_pDigimon_Manager->PlayerPos(), 30, fTimeDelta))
		{
			m_pFsm->Enter(DIGIMONSTATE::RUN, m_pPart_Body);
			m_bMove = true;
		}

		if (!m_bMove)
			m_pFsm->Enter(DIGIMONSTATE::STAND, m_pPart_Body);
	}


	m_pFsm->Update(fTimeDelta);

	__super::Update(fTimeDelta);
}

void CLeomon::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CLeomon::Render()
{
	return S_OK;
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
	Safe_Release(m_pPart_Body);

}
