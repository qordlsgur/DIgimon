#include "MetalgarumonSkill2.h"
#include "GameInstance.h"
#include "Interaction_Manager.h"

CMetalgarumonSkill2::CMetalgarumonSkill2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CSkillObject{ pDevice, pContext }
{
}

CMetalgarumonSkill2::CMetalgarumonSkill2(const CMetalgarumonSkill2& Prototype)
    : CSkillObject{ Prototype }
{
}

HRESULT CMetalgarumonSkill2::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMetalgarumonSkill2::Initialize(void* pArg)
{
    return E_NOTIMPL;
}

void CMetalgarumonSkill2::Priority_Update(_float fTimeDelta)
{
}

void CMetalgarumonSkill2::Update(_float fTimeDelta)
{
}

void CMetalgarumonSkill2::Late_Update(_float fTimeDelta)
{
}

HRESULT CMetalgarumonSkill2::Render()
{
#ifdef _DEBUG
	m_pColliderCom->Render();
#endif

	return S_OK;
}

_int CMetalgarumonSkill2::Get_Damage()
{
	return 
		m_iDamage;
}

HRESULT CMetalgarumonSkill2::Ready_PartObjects()
{
	/* Com_Sphere*/
	CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};
	SphereDesc.fRadius = 5.f;
	SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius, 0.f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider_Sphere"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
		return E_FAIL;

	m_pInteraction_Manager->Set_Skill_Collider(m_pColliderCom);
	m_pColliderCom->Set_Matrix(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));

	return S_OK;;
}

CMetalgarumonSkill2* CMetalgarumonSkill2::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMetalgarumonSkill2* pInstance = new CMetalgarumonSkill2(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMetalgarumonSkill2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMetalgarumonSkill2::Clone(void* pArg)
{
	CMetalgarumonSkill2* pInstance = new CMetalgarumonSkill2(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMetalgarumonSkill2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMetalgarumonSkill2::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);

}