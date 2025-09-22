#include "Blackwargreymon.h"
#include "GameInstance.h"
#include "Body_Blackwargreymon.h"
#include "PartObject.h"
#include "StateMachine.h"
#include "Digimon_Manager.h"

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

	Digimon_Info Info;

	Info.DigimonName = TEXT("블렉워그레이몬");
	Info.DigimonId = 1;
	Info.Stage = DIGIMON_STAGE::MEGA;
	Info.Attribute = DIGIMON_ATTRIBUTE::VIRUS;
	Info.DigimonInfo = TEXT("칠흑의 용 전사 라고 불리는 두려운 존재인 바이러스종 워그레이몬");
	Info.Hp = 7000;
	Info.Sp = 130;
	Info.Damage = 1000;
	Info.AttackSpeed = 130;
	Info.Exp = 0;
	Info.Lv = 90;

	__super::Set_Digimon_Info(Info);
	m_pDigimon_Manager->Digimon_Add(Info.DigimonId, Info);

	return S_OK;
}

void CBlackwargreymon::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CBlackwargreymon::Update(_float fTimeDelta)
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


void CBlackwargreymon::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBlackwargreymon::Render()
{
	return S_OK;
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
	Safe_Release(m_pPart_Body);

}
