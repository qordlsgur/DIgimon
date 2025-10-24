#include "MetalgarumonSkill3.h"
#include "GameInstance.h"
#include "Interaction_Manager.h"

CMetalgarumonSkill3::CMetalgarumonSkill3(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CSkillObject{ pDevice, pContext }
{
}

CMetalgarumonSkill3::CMetalgarumonSkill3(const CMetalgarumonSkill3& Prototype)
    : CSkillObject{ Prototype }
{
}

HRESULT CMetalgarumonSkill3::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMetalgarumonSkill3::Initialize(void* pArg)
{
    return E_NOTIMPL;
}

void CMetalgarumonSkill3::Priority_Update(_float fTimeDelta)
{
}

void CMetalgarumonSkill3::Update(_float fTimeDelta)
{
}

void CMetalgarumonSkill3::Late_Update(_float fTimeDelta)
{
}

HRESULT CMetalgarumonSkill3::Render()
{
#ifdef _DEBUG
	m_pColliderCom->Render();
#endif

	return S_OK;
}

_int CMetalgarumonSkill3::Get_Damage()
{
	return m_iDamage;
}

HRESULT CMetalgarumonSkill3::Ready_PartObjects()
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

CMetalgarumonSkill3* CMetalgarumonSkill3::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMetalgarumonSkill3* pInstance = new CMetalgarumonSkill3(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMetalgarumonSkill3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMetalgarumonSkill3::Clone(void* pArg)
{
	CMetalgarumonSkill3* pInstance = new CMetalgarumonSkill3(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMetalgarumonSkill3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMetalgarumonSkill3::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);

}