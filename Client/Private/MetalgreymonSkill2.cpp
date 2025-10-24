#include "MetalgreymonSkill2.h"
#include "GameInstance.h"
#include "Interaction_Manager.h"

CMetalgreymonSkill2::CMetalgreymonSkill2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CSkillObject{ pDevice, pContext }
{
}

CMetalgreymonSkill2::CMetalgreymonSkill2(const CMetalgreymonSkill2& Prototype)
	: CSkillObject{ Prototype }
{
}

HRESULT CMetalgreymonSkill2::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMetalgreymonSkill2::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC	Desc{};
	Desc.fRotationPerSec = XMConvertToRadians(180.0f);
	Desc.fSpeedPerSec = 500.f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	m_pInteraction_Manager = CInteraction_Manager::GetInstance();
	m_pInteraction_Manager->Set_Attack_Digimon(this);

	const POSITION* Pos = static_cast<const POSITION*>(pArg);

	m_iDamage = Pos->iDamage;
	m_vPosition = Pos->m_vPosition;
	m_vTarget_pos = Pos->m_vTargetPosition;

	m_vPosition.m128_f32[1] += 10.f;
	if (Pos->Look == 0)
	{
		m_vPosition.m128_f32[2] -= 10.f;
	}
	else if (Pos->Look == 1)
	{
		m_vPosition.m128_f32[2] += 10.f;
	}

	m_pTransformCom->Set_State(STATE::POSITION, m_vPosition);

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	return S_OK;
}
void CMetalgreymonSkill2::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CMetalgreymonSkill2::Update(_float fTimeDelta)
{
	m_pTransformCom->Target_Pos_Move(m_vTarget_pos, fTimeDelta);

	m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));

	__super::Update(fTimeDelta);
}

void CMetalgreymonSkill2::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::UI, this);
	__super::Late_Update(fTimeDelta);
}


HRESULT CMetalgreymonSkill2::Render()
{
#ifdef _DEBUG
	m_pColliderCom->Render();
#endif

	return S_OK;
}

_int CMetalgreymonSkill2::Get_Damage()
{
	return m_iDamage;
}

HRESULT CMetalgreymonSkill2::Ready_PartObjects()
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

CMetalgreymonSkill2* CMetalgreymonSkill2::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMetalgreymonSkill2* pInstance = new CMetalgreymonSkill2(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMetalgreymonSkill2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMetalgreymonSkill2::Clone(void* pArg)
{
	CMetalgreymonSkill2* pInstance = new CMetalgreymonSkill2(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMetalgreymonSkill2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMetalgreymonSkill2::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);

}