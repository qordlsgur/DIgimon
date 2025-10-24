#include "DevilmonSkill3.h"
#include "GameInstance.h"
#include "Interaction_Manager.h"

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
    return E_NOTIMPL;
}

void CDevilmonSkill3::Priority_Update(_float fTimeDelta)
{
}

void CDevilmonSkill3::Update(_float fTimeDelta)
{
}

void CDevilmonSkill3::Late_Update(_float fTimeDelta)
{
}

HRESULT CDevilmonSkill3::Render()
{
#ifdef _DEBUG
	m_pColliderCom->Render();
#endif

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
	m_pColliderCom->Set_Matrix(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));

	return S_OK;;
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