#include "Wargreymon.h"
#include "GameInstance.h"
#include "Body_Wargreymon.h"
#include "PartObject.h"
#include "StateMachine.h"
#include "Digimon_Manager.h"

CWargreymon::CWargreymon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CContainerObject{ pDevice, pContext }
{
}

CWargreymon::CWargreymon(const CWargreymon& Prototype)
	: CContainerObject{ Prototype }
{
}

HRESULT CWargreymon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWargreymon::Initialize(void* pArg)
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

	Info.DigimonName = TEXT("워그레이몬");
	Info.DigimonId = 0;
	Info.Stage = DIGIMON_STAGE::MEGA;
	Info.Attribute = DIGIMON_ATTRIBUTE::VACCINE;
	Info.DigimonInfo = TEXT("초금속 크롬디지조이드의 값옷을 입고 있는 최강의 용전사");
	Info.Hp = 7000;
	Info.Sp = 130;
	Info.Damage = 1000;
	Info.AttackSpeed = 130;
	Info.Exp = 0;
	Info.Lv = 90;

	__super::Set_Digimon_Info(Info);

	return S_OK;
}

void CWargreymon::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CWargreymon::Update(_float fTimeDelta)
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

void CWargreymon::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CWargreymon::Render()
{
	return S_OK;
}

HRESULT CWargreymon::Ready_PartObjects()
{
	CBody_Wargreymon::BODY_PLAYER_DESC BodyDesc{};

	BodyDesc.pParentTransform = m_pTransformCom;

	/* Part_Body */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Wargreymon"),
		TEXT("Part_Body_Wargreymon"), &BodyDesc)))
		return E_FAIL;

	m_pPart_Body = dynamic_cast<CBody_Wargreymon*>(Find_PartObject(TEXT("Part_Body_Wargreymon")));

	return S_OK;
}

CWargreymon* CWargreymon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWargreymon* pInstance = new CWargreymon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWargreymon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWargreymon::Clone(void* pArg)
{
	CWargreymon* pInstance = new CWargreymon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWargreymon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWargreymon::Free()
{
	__super::Free();

	Safe_Release(m_pFsm);
	Safe_Release(m_pPart_Body);

}
