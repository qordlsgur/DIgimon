#include "AngewomonSkill2.h"
#include "GameInstance.h"
#include "Interaction_Manager.h"

CAngewomonSkill2::CAngewomonSkill2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSkillObject{ pDevice, pContext }
{
}

CAngewomonSkill2::CAngewomonSkill2(const CAngewomonSkill2& Prototype)
	: CSkillObject{ Prototype }
{
}

HRESULT CAngewomonSkill2::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAngewomonSkill2::Initialize(void* pArg)
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
	m_vTarget_pos.m128_f32[1] += 10.f;
	if (Pos->Look == 0)
	{
		m_vPosition.m128_f32[2] -= 10.f;
		m_vTarget_pos.m128_f32[2] -= 5.f;
	}
	else if(Pos->Look == 1)
	{
		m_vPosition.m128_f32[2] += 10.f;
		m_vTarget_pos.m128_f32[2] += 5.f;
	}

	m_pTransformCom->Set_State(STATE::POSITION, m_vPosition);

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	return S_OK;
}

void CAngewomonSkill2::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CAngewomonSkill2::Update(_float fTimeDelta)
{
	m_pTransformCom->Target_Pos_Move(m_vTarget_pos, fTimeDelta);

	m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));

	__super::Update(fTimeDelta);
}

void CAngewomonSkill2::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::UI, this);
	__super::Late_Update(fTimeDelta);
}

HRESULT CAngewomonSkill2::Render()
{
#ifdef _DEBUG
	m_pColliderCom->Render();
#endif

	return S_OK;
}

_int CAngewomonSkill2::Get_Damage()
{
	return m_iDamage;
}

HRESULT CAngewomonSkill2::Ready_PartObjects()
{
	/* Com_Sphere*/
	CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};
	SphereDesc.fRadius = 5.f;
	SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius, 0.f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider_Sphere"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
		return E_FAIL;

	m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));
	m_pInteraction_Manager->Set_Skill_Collider(m_pColliderCom);

	return S_OK;
}

CAngewomonSkill2* CAngewomonSkill2::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CAngewomonSkill2* pInstance = new CAngewomonSkill2(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CAngewomonSkill2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CAngewomonSkill2::Clone(void* pArg)
{
	CAngewomonSkill2* pInstance = new CAngewomonSkill2(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CAngewomonSkill2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAngewomonSkill2::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);

}
