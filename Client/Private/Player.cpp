#include "Player.h"
#include "GameInstance.h"
#include "Body_Player.h"
#include "PartObject.h"
#include "Camera_Manager.h"
#include "StateMachine.h"
#include "Digimon_Manager.h"
#include "Navigation.h"
#include "Battle_Manager.h"
#include "Interaction_Manager.h"

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CContainerObject{ pDevice, pContext }
{
}

CPlayer::CPlayer(const CPlayer& Prototype)
	: CContainerObject{ Prototype }
{
}

HRESULT CPlayer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC	Desc{};
	Desc.fRotationPerSec = XMConvertToRadians(180.0f);
	Desc.fSpeedPerSec = 30.f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	m_pCamera_Manager = CCamera_Manager::GetInstance();
	m_pCamera_Manager->Add_Player(this);

	m_pDigimon_Manager = CDigimon_Manager::GetInstance();
	m_pDigimon_Manager->Player(this);

	m_pBattle_Manager = CBattle_Manager::GetInstance();

	m_pInteraction_Manager = CInteraction_Manager::GetInstance();

	m_pFsm = CStateMachine::Create();

	m_pFsm->Initialize();

	m_pFsm->Enter(DIGIMONSTATE::STAND, m_pPart_Body, false, false);

	m_fGravity = 16.8f;
	m_fJumpPower = 4.5f;
	m_fJumpTime = 0.1f;
	m_fMax_Jump = 30.f;

	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(10.f, 0.f, 200.f, 1.f));

	m_eCurrentState = PLAYER_STATE::STAND;
	m_ePreviousState = PLAYER_STATE::STAND;

	m_pInteraction_Manager->Set_Player(this);

	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
	m_pFsm->Update(fTimeDelta);
}

void CPlayer::Update(_float fTimeDelta)
{
	m_bBattle = m_pBattle_Manager->Get_Battle();
	if (!m_FindCell)
	{
		m_pNavigationCom->Find_Cell(m_pTransformCom->Get_State(STATE::POSITION));
		m_FindCell = true;
	}

	if (!m_bBattle)
	{
		_vector Angle = XMVectorZero();

		if (m_pGameInstance->Key_Down(DIK_SPACE))
		{
			m_fJumpStart = 0;
			m_pFsm->Enter(DIGIMONSTATE::JUMP, m_pPart_Body, false, false);
			m_bisJump = true;
		}

		m_bMove = false;
		if (m_pGameInstance->Key_Pressing(DIK_W))
		{
			m_pFsm->Enter(DIGIMONSTATE::RUN, m_pPart_Body);
			Angle = XMVectorAdd(Angle, XMVectorSet(0.f, 0.f, 1.f, 0.f));
			m_bMove = true;
		}

		if (m_pGameInstance->Key_Pressing(DIK_S))
		{
			m_pFsm->Enter(DIGIMONSTATE::RUN, m_pPart_Body);
			Angle = XMVectorAdd(Angle, XMVectorSet(0.f, 0.f, -1.f, 0.f));
			m_bMove = true;
		}

		if (m_pGameInstance->Key_Pressing(DIK_A))
		{
			m_pFsm->Enter(DIGIMONSTATE::RUN, m_pPart_Body);
			Angle = XMVectorAdd(Angle, XMVectorSet(-1.f, 0.f, 0.f, 0.f));
			m_bMove = true;
		}

		if (m_pGameInstance->Key_Pressing(DIK_D))
		{
			m_pFsm->Enter(DIGIMONSTATE::RUN, m_pPart_Body);
			Angle = XMVectorAdd(Angle, XMVectorSet(1.f, 0.f, 0.f, 0.f));
			m_bMove = true;
		}

		if (m_bMove)
		{
			m_eCurrentState = PLAYER_STATE::MOVE;
			if (!XMVector3Equal(Angle, XMVectorZero()))
				XMVector3Normalize(Angle);

			_matrix CameraY = XMMatrixRotationY(m_pCamera_Manager->Get_Angle());
			_vector Pos = XMVector3TransformNormal(Angle, CameraY);

			m_pTransformCom->Look(Pos, fTimeDelta);
			m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);
		}

		if (!m_bMove && !m_bisJump)
		{
			m_eCurrentState = PLAYER_STATE::STAND;
			m_pFsm->Enter(DIGIMONSTATE::STAND, m_pPart_Body);
			m_pNavigationCom->Compute_Height(m_pTransformCom);
		}

		if (m_bisJump)
			Jump(fTimeDelta);
	}

	else
	{
		m_eCurrentState = PLAYER_STATE::STAND;
		m_pFsm->Enter(DIGIMONSTATE::STAND, m_pPart_Body);
		LookAt(-180.f);
		Set_Position(100.f, 140.f);
	}
	m_pFsm->Update(fTimeDelta);
	m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));

	__super::Update(fTimeDelta);
}

void CPlayer::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);

}

HRESULT CPlayer::Render()
{
#ifdef _DEBUG
	m_pNavigationCom->Render();
	m_pColliderCom->Render();
#endif

	return S_OK;
}

_bool CPlayer::Intersect()
{
	return _bool();
}

void CPlayer::Jump(_float fTimeDelta)
{
	m_fJumpTime += fTimeDelta;

	m_fHight = (m_fJumpTime * m_fJumpTime * (-m_fGravity) * 0.5f) + (m_fJumpTime * m_fJumpPower);
	m_pTransformCom->Jump(m_fHight);

	if (m_fMax_Jump <= m_fHight)
		m_fMax_Jump = m_fHight;

	else
	{
		if (m_pTransformCom->Get_State(STATE::POSITION).m128_f32[1] <= 0.f)
		{
			m_pTransformCom->Set_Y(m_fJumpStart);
			m_bisJump = false;
			m_fJumpTime = 0.f;
			m_fMax_Jump = -30.f;
		}
	}
}

void CPlayer::LockCamera()
{
	if (m_pCamera_Manager->Get_LcokCamera())
	{
		m_pTransformCom->Set_State(STATE::LOOK, XMVector4Normalize(XMLoadFloat4(m_pGameInstance->Get_CamPosition())));
	}
}

HRESULT CPlayer::Ready_Components()
{
	/* Com_Navigation */
	CNavigation::NAVIGATION_DESC		NavigationDesc{};
	NavigationDesc.iCurrentCellIndex = 150;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NavigationDesc)))
		return E_FAIL;

	/* Com_Sphere*/
	CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};
	SphereDesc.fRadius = 5.f;
	SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius, 0.f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider_Sphere"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CPlayer::Ready_PartObjects()
{
	CBody_Player::BODY_PLAYER_DESC BodyDesc{};

	BodyDesc.pParentTransform = m_pTransformCom;

	/* Part_Body */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Player"),
		TEXT("Part_Body"), &BodyDesc)))
		return E_FAIL;

	m_pPart_Body = dynamic_cast<CBody_Player*>(Find_PartObject(TEXT("Part_Body")));

	return S_OK;
}

CPlayer* CPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer* pInstance = new CPlayer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pFsm);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pPart_Body);
}
