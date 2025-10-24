#include "LadydevimonSkill3.h"
#include "GameInstance.h"
#include "Interaction_Manager.h"

CLadydevimonSkill3::CLadydevimonSkill3(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CSkillObject{ pDevice, pContext }
{
}

CLadydevimonSkill3::CLadydevimonSkill3(const CLadydevimonSkill3& Prototype)
    : CSkillObject{ Prototype }
{
}

HRESULT CLadydevimonSkill3::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CLadydevimonSkill3::Initialize(void* pArg)
{
    return E_NOTIMPL;
}

void CLadydevimonSkill3::Priority_Update(_float fTimeDelta)
{
}

void CLadydevimonSkill3::Update(_float fTimeDelta)
{
}

void CLadydevimonSkill3::Late_Update(_float fTimeDelta)
{
}

HRESULT CLadydevimonSkill3::Render()
{
#ifdef _DEBUG
	m_pColliderCom->Render();
#endif

	return S_OK;
}

_int CLadydevimonSkill3::Get_Damage()
{
	return m_iDamage;
}

HRESULT CLadydevimonSkill3::Ready_PartObjects()
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

CLadydevimonSkill3* CLadydevimonSkill3::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLadydevimonSkill3* pInstance = new CLadydevimonSkill3(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLadydevimonSkill3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLadydevimonSkill3::Clone(void* pArg)
{
	CLadydevimonSkill3* pInstance = new CLadydevimonSkill3(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CLadydevimonSkill3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLadydevimonSkill3::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);

}