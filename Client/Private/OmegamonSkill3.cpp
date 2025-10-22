#include "OmegamonSkill3.h"
#include "GameInstance.h"
#include "Interaction_Manager.h"

COmegamonSkill3::COmegamonSkill3(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSkillObject{ pDevice, pContext }
{
}

COmegamonSkill3::COmegamonSkill3(const COmegamonSkill3& Prototype)
	: CSkillObject{ Prototype }
{
}

HRESULT COmegamonSkill3::Initialize_Prototype()
{
	return S_OK;
}

HRESULT COmegamonSkill3::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC	Desc{};
	Desc.fRotationPerSec = XMConvertToRadians(180.0f);
	Desc.fSpeedPerSec = 10.f;

	m_fSpeed = 50.f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	m_pInteraction_Manager = CInteraction_Manager::GetInstance();
	m_pInteraction_Manager->Set_Attack_Digimon(this);
	m_pInteraction_Manager->Set_Skill_Collider(m_pColliderCom);

	const POSITION* Pos = static_cast<const POSITION*>(pArg);

	m_iDamage = Pos->iDamage;

	m_vPosition = Pos->m_vPosition;

	m_vTarget_pos = Pos->m_vTargetPosition;

	m_pTransformCom->Set_State(STATE::POSITION, m_vPosition);

	return S_OK;
}

void COmegamonSkill3::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void COmegamonSkill3::Update(_float fTimeDelta)
{
	Pos = m_pTransformCom->Get_State(STATE::POSITION);

	_vector Dir = XMVectorSubtract(m_vTarget_pos, Pos);
	_float distance = XMVectorGetX(XMVector3Length(Dir));

	_vector dirNormalized = XMVector3Normalize(Dir);
	_vector move = XMVectorScale(dirNormalized, m_fSpeed * fTimeDelta * 3.5f);

	Pos = XMVectorAdd(Pos, move); // ÀÌµ¿

	m_pTransformCom->Set_State((STATE::POSITION), Pos);

	m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));

	__super::Update(fTimeDelta);
}

void COmegamonSkill3::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
	__super::Late_Update(fTimeDelta);
}

HRESULT COmegamonSkill3::Render()
{
#ifdef _DEBUG
	m_pColliderCom->Render();
#endif

	return S_OK;
}

void COmegamonSkill3::Set_Pos(_float fX, _float fY)
{
}

_int COmegamonSkill3::Get_Damage()
{
	return m_iDamage;
}

HRESULT COmegamonSkill3::Ready_PartObjects()
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

COmegamonSkill3* COmegamonSkill3::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	COmegamonSkill3* pInstance = new COmegamonSkill3(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : COmegamonSkill3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* COmegamonSkill3::Clone(void* pArg)
{
	COmegamonSkill3* pInstance = new COmegamonSkill3(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : COmegamonSkill3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void COmegamonSkill3::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);

}
