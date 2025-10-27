#include "LeomonSkill2.h"
#include "GameInstance.h"
#include "Interaction_Manager.h"
#include "LeomonSkill2_Part1.h"

CLeomonSkill2::CLeomonSkill2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSkillObject{ pDevice, pContext }
{
}

CLeomonSkill2::CLeomonSkill2(const CLeomonSkill2& Prototype)
	: CSkillObject{ Prototype }
{
}

HRESULT CLeomonSkill2::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLeomonSkill2::Initialize(void* pArg)
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

	m_vTarget_pos.m128_f32[1] += 5.f;

	m_pTransformCom->Set_State(STATE::POSITION, m_vPosition);

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	if (FAILED(Ready_SkillObjects()))
		return E_FAIL;

	return S_OK;
}

void CLeomonSkill2::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CLeomonSkill2::Update(_float fTimeDelta)
{
	m_pTransformCom->TargetLook(m_vTarget_pos);

	m_pTransformCom->Target_Pos_Move(m_vTarget_pos, fTimeDelta);

	m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));

	__super::Update(fTimeDelta);
}

void CLeomonSkill2::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::UI, this);
	__super::Late_Update(fTimeDelta);
}

HRESULT CLeomonSkill2::Render()
{
#ifdef _DEBUG
	m_pColliderCom->Render();
#endif

	return S_OK;
}

void CLeomonSkill2::Set_Pos(_float fX, _float fY)
{
}

_int CLeomonSkill2::Get_Damage()
{
	return m_iDamage;
}

HRESULT CLeomonSkill2::Ready_PartObjects()
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

	return S_OK;
}

HRESULT CLeomonSkill2::Ready_SkillObjects()
{
	CLeomonSkill2_Part1::BODY_PLAYER_DESC Skill1{};

	Skill1.pParentTransform = m_pTransformCom;

	/* Part_Skill1 */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_LeomonSkill2_Part1"),
		TEXT("Part_Skill1"), &Skill1)))
		return E_FAIL;

	m_pSkillModel1 = dynamic_cast<CLeomonSkill2_Part1*>(Find_PartObject(TEXT("Part_Skill1")));
	return S_OK;
}

CLeomonSkill2* CLeomonSkill2::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLeomonSkill2* pInstance = new CLeomonSkill2(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLeomonSkill2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLeomonSkill2::Clone(void* pArg)
{
	CLeomonSkill2* pInstance = new CLeomonSkill2(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CLeomonSkill2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLeomonSkill2::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);

}
