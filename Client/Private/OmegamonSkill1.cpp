#include "OmegamonSkill1.h"
#include "GameInstance.h"
#include "Interaction_Manager.h"

COmegamonSkill1::COmegamonSkill1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSkillObject{ pDevice, pContext }
{
}

COmegamonSkill1::COmegamonSkill1(const COmegamonSkill1& Prototype)
	: CSkillObject{ Prototype }
{
}

HRESULT COmegamonSkill1::Initialize_Prototype()
{
	return S_OK;
}

HRESULT COmegamonSkill1::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC	Desc{};
	Desc.fRotationPerSec = XMConvertToRadians(180.0f);
	Desc.fSpeedPerSec = 10.f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	m_pInteraction_Manager = CInteraction_Manager::GetInstance();
	m_pInteraction_Manager->Set_Attack_Digimon(this);
	m_pInteraction_Manager->Set_Skill_Collider(m_pColliderCom);

	const POSITION* Pos = static_cast<const POSITION*>(pArg);

	m_iDamage = Pos->iDamage;

	m_vTarget_pos = Pos->m_vTargetPosition;

	m_pTransformCom->Set_State(STATE::POSITION, m_vTarget_pos);

	return S_OK;
}

void COmegamonSkill1::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void COmegamonSkill1::Update(_float fTimeDelta)
{
	m_pTransformCom->Set_State((STATE::POSITION), m_vTarget_pos);

	m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));

	__super::Update(fTimeDelta);
}

void COmegamonSkill1::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
	__super::Late_Update(fTimeDelta);
}

HRESULT COmegamonSkill1::Render()
{
#ifdef _DEBUG
	m_pColliderCom->Render();
#endif

	return S_OK;
}

void COmegamonSkill1::Set_Pos(_float fX, _float fY)
{
}

_int COmegamonSkill1::Get_Damage()
{
	return m_iDamage;
}

HRESULT COmegamonSkill1::Ready_PartObjects()
{
	/* Com_Sphere*/
	CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};
	SphereDesc.fRadius = 5.f;
	SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius, 0.f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider_Sphere"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
		return E_FAIL;

	return S_OK;
}

COmegamonSkill1* COmegamonSkill1::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	COmegamonSkill1* pInstance = new COmegamonSkill1(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : COmegamonSkill1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* COmegamonSkill1::Clone(void* pArg)
{
	COmegamonSkill1* pInstance = new COmegamonSkill1(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : COmegamonSkill1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void COmegamonSkill1::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);

}
