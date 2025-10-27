#include "DevilmonSkill3.h"
#include "GameInstance.h"
#include "Interaction_Manager.h"
#include "DevilmonSkill3_Part1.h"
#include "DevilmonSkill3_Part2.h"
#include "DevilmonSkill3_Part3.h"
#include "DevilmonSkill3_Part4.h"

CDevilmonSkill3::CDevilmonSkill3(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CSkillObject{ pDevice, pContext }
{
}

CDevilmonSkill3::CDevilmonSkill3(const CDevilmonSkill3& Prototype)
    : CSkillObject{ Prototype }
{
}

HRESULT CDevilmonSkill3::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CDevilmonSkill3::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC	Desc{};
	Desc.fRotationPerSec = XMConvertToRadians(180.0f);
	Desc.fSpeedPerSec = 10.f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;


	m_pInteraction_Manager = CInteraction_Manager::GetInstance();
	m_pInteraction_Manager->Set_Attack_Digimon(this);


	const POSITION* Pos = static_cast<const POSITION*>(pArg);

	m_iDamage = Pos->iDamage;
	m_vPosition = Pos->m_vPosition;
	m_vTarget_pos = Pos->m_vTargetPosition;
	m_pTransformCom->Set_State(STATE::POSITION, m_vPosition);
	m_pTransformCom->Update_WoldMatrix();

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	if (FAILED(Ready_SkillObjects()))
		return E_FAIL;

	return S_OK;
}

void CDevilmonSkill3::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CDevilmonSkill3::Update(_float fTimeDelta)
{
	m_pTransformCom->Set_State((STATE::POSITION), m_vPosition);


	if (m_bCharge)
	{
		//Ready_Skill2Objects();
		m_bCharge = false;
	}

	if (m_pLeftHand != nullptr && m_pRightHand != nullptr)
	{
		m_pLeftHand->Set_Position(m_vLeft);
		m_pRightHand->Set_Position(m_vRight);
	}

	if (m_bMove == true)
	{
		Ready_Skill3Objects();
	}

	__super::Update(fTimeDelta);
}

void CDevilmonSkill3::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
	__super::Late_Update(fTimeDelta);
}

HRESULT CDevilmonSkill3::Render()
{


	return S_OK;
}

_int CDevilmonSkill3::Get_Damage()
{
	return m_iDamage;
}

HRESULT CDevilmonSkill3::Ready_PartObjects()
{
	/* Com_Sphere*/
	CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};
	SphereDesc.fRadius = 5.f;
	SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius, 0.f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider_Sphere"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
		return E_FAIL;

	m_pInteraction_Manager->Set_Skill_Collider(m_pColliderCom);
	m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));

	return S_OK;;
}

HRESULT CDevilmonSkill3::Ready_SkillObjects()
{
	CDevilmonSkill3_Part1::BODY_PLAYER_DESC Skill1{};

	Skill1.pParentTransform = m_pTransformCom;

	/* Part_Skill1 */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_DevilmonSkill3_Part1"),
		TEXT("Part_Skill1"), &Skill1)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDevilmonSkill3::Ready_Skill2Objects()
{
	CDevilmonSkill3_Part2::BODY_PLAYER_DESC Skill2{};

	Skill2.pParentTransform = m_pTransformCom;
	Skill2.vMatrix = m_vLeft;

	/* Part_Skill1 */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_DevilmonSkill3_Part2"),
		TEXT("Part_Skill2"), &Skill2)))
		return E_FAIL;

	m_pLeftHand = dynamic_cast<CDevilmonSkill3_Part2*>(Find_PartObject(TEXT("Part_Skill2")));

	CDevilmonSkill3_Part3::BODY_PLAYER_DESC Skill3{};
	
	Skill3.pParentTransform = m_pTransformCom;
	Skill3.vMatrix = m_vRight;


	/* Part_Skill1 */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_DevilmonSkill3_Part3"),
		TEXT("Part_Skill3"), &Skill3)))
		return E_FAIL;

	m_pRightHand = dynamic_cast<CDevilmonSkill3_Part3*>(Find_PartObject(TEXT("Part_Skill3")));

	return S_OK;
}

HRESULT CDevilmonSkill3::Ready_Skill3Objects()
{
	CDevilmonSkill3_Part4::BODY_PLAYER_DESC Skill4{};

	//Skill4.vMatrix = m_pTransformCom->Get_S;
	Skill4.vPosition = m_vPosition;
	Skill4.vPosition2 = m_vTarget_pos;

	/* Part_Skill4 */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_DevilmonSkill3_Part4"),
		TEXT("Part_Skill4"), &Skill4)))
		return E_FAIL;

	return S_OK;
}

CDevilmonSkill3* CDevilmonSkill3::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDevilmonSkill3* pInstance = new CDevilmonSkill3(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDevilmonSkill3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDevilmonSkill3::Clone(void* pArg)
{
	CDevilmonSkill3* pInstance = new CDevilmonSkill3(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDevilmonSkill3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDevilmonSkill3::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);

}