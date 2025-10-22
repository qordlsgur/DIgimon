#include "AngewomonSkill1.h"
#include "GameInstance.h"
#include "Interaction_Manager.h"

CAngewomonSkill1::CAngewomonSkill1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSkillObject{ pDevice, pContext }
{
}

CAngewomonSkill1::CAngewomonSkill1(const CAngewomonSkill1& Prototype)
	: CSkillObject{ Prototype }
{
}

HRESULT CAngewomonSkill1::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAngewomonSkill1::Initialize(void* pArg)
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


void CAngewomonSkill1::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CAngewomonSkill1::Update(_float fTimeDelta)
{
	m_pTransformCom->Set_State((STATE::POSITION), m_vTarget_pos);

	m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));

	__super::Update(fTimeDelta);
}

void CAngewomonSkill1::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
	__super::Late_Update(fTimeDelta);
}

HRESULT CAngewomonSkill1::Render()
{
#ifdef _DEBUG
	m_pColliderCom->Render();
#endif

	return S_OK;
}

_int CAngewomonSkill1::Get_Damage()
{
	return m_iDamage;
}

HRESULT CAngewomonSkill1::Ready_PartObjects()
{
	/* Com_Sphere*/
	CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};
	SphereDesc.fRadius = 5.f;
	SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius, 0.f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider_Sphere"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
		return E_FAIL;

	return S_OK;;
}

CAngewomonSkill1* CAngewomonSkill1::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CAngewomonSkill1* pInstance = new CAngewomonSkill1(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CAngewomonSkill1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CAngewomonSkill1::Clone(void* pArg)
{
	CAngewomonSkill1* pInstance = new CAngewomonSkill1(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CAngewomonSkill1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAngewomonSkill1::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);

}