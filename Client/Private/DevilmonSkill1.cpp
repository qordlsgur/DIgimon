#include "DevilmonSkill1.h"
#include "GameInstance.h"
#include "Interaction_Manager.h"

CDevilmonSkill1::CDevilmonSkill1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CSkillObject{ pDevice, pContext }
{
}

CDevilmonSkill1::CDevilmonSkill1(const CDevilmonSkill1& Prototype)
    : CSkillObject{ Prototype }
{
}

HRESULT CDevilmonSkill1::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CDevilmonSkill1::Initialize(void* pArg)
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

	m_vTarget_pos = Pos->m_vTargetPosition;

	m_pTransformCom->Set_State(STATE::POSITION, m_vTarget_pos);

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	return S_OK;
}


void CDevilmonSkill1::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CDevilmonSkill1::Update(_float fTimeDelta)
{
	m_pTransformCom->Set_State((STATE::POSITION), m_vTarget_pos);

	m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));

	__super::Update(fTimeDelta);
}

void CDevilmonSkill1::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
	__super::Late_Update(fTimeDelta);
}

HRESULT CDevilmonSkill1::Render()
{
#ifdef _DEBUG
	m_pColliderCom->Render();
#endif

	return S_OK;
}

_int CDevilmonSkill1::Get_Damage()
{
	return m_iDamage;
}

HRESULT CDevilmonSkill1::Ready_PartObjects()
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

CDevilmonSkill1* CDevilmonSkill1::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDevilmonSkill1* pInstance = new CDevilmonSkill1(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDevilmonSkill1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDevilmonSkill1::Clone(void* pArg)
{
	CDevilmonSkill1* pInstance = new CDevilmonSkill1(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDevilmonSkill1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDevilmonSkill1::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);

}