#include "MetalgreymonSkill2.h"
#include "GameInstance.h"
#include "Interaction_Manager.h"
#include "MetalgreymonSkill2_Part1.h"
#include "MetalgreymonSkill2_Part2.h"
#include "MetalgreymonSkill2_Part3.h"

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
	Desc.fSpeedPerSec = 200.f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	m_pInteraction_Manager = CInteraction_Manager::GetInstance();
	m_pInteraction_Manager->Set_Attack_Digimon(this);

	const POSITION* Pos = static_cast<const POSITION*>(pArg);

	m_iDamage = Pos->iDamage;
	m_vPosition = Pos->m_vPosition;
	m_vPosition2 = Pos->m_vPosition2;
	m_vTarget_pos = Pos->m_vTargetPosition;
	m_vPosition.m128_f32[1] += 10.f;
	if (Pos->Look == 0)
	{
		m_vPosition.m128_f32[2] -= 5.f;
	}
	else if (Pos->Look == 1)
	{
		m_vPosition.m128_f32[2] += 5.f;
	}
	m_vTarget_pos.m128_f32[1] += 10.f;
	m_pTransformCom->Set_State(STATE::POSITION, m_vPosition);

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	if (FAILED(Ready_SkillObjects()))
		return E_FAIL;

	return S_OK;
}
void CMetalgreymonSkill2::Priority_Update(_float fTimeDelta)
{

	__super::Priority_Update(fTimeDelta);
}

void CMetalgreymonSkill2::Update(_float fTimeDelta)
{

	if (!m_pTransformCom->Target_Pos_Move_Bool(m_vTarget_pos, fTimeDelta))
	{
		m_pSkillModel1->Set_Hit(true);
		Ready_Skill3Objects();
		m_fTime += fTimeDelta;
		//m_isDead = true;
	}


	if (m_fTime > 1.f)
	{
		m_pInteraction_Manager->Die_Attack_Skill();
		m_isDead = true;
	}
	m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));

	m_pTransformCom->TargetLook(m_vTarget_pos);


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

HRESULT CMetalgreymonSkill2::Ready_SkillObjects()
{
	CMetalgreymonSkill2_Part1::BODY_PLAYER_DESC Skill1{};

	Skill1.pParentTransform = m_pTransformCom;
	Skill1.vPosition = m_vTarget_pos;

	/* Part_Skill1 */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgreymonSkill2_Part1"),
		TEXT("Part_Skill1"), &Skill1)))
		return E_FAIL;

	m_pSkillModel1 = dynamic_cast<CMetalgreymonSkill2_Part1*>(Find_PartObject(TEXT("Part_Skill1")));

	CMetalgreymonSkill2_Part2::BODY_PLAYER_DESC Skill2{};


	Skill2.pParentTransform = m_pTransformCom;
	Skill2.vPosition = m_vTarget_pos;

	/* Part_Skill2 */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgreymonSkill2_Part2"),
		TEXT("Part_Skill2"), &Skill2)))
		return E_FAIL;

	m_pSkillModel2 = dynamic_cast<CMetalgreymonSkill2_Part2*>(Find_PartObject(TEXT("Part_Skill2")));
	return S_OK;
}

HRESULT CMetalgreymonSkill2::Ready_Skill3Objects()
{
	CMetalgreymonSkill2_Part1::BODY_PLAYER_DESC Skil13{};

	Skil13.pParentTransform = m_pTransformCom;
	Skil13.vPosition = m_vTarget_pos;

	/* Part_Skill2 */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgreymonSkill2_Part3"),
		TEXT("Part_Skill3"), &Skil13)))
		return E_FAIL;

	m_pSkillModel3 = dynamic_cast<CMetalgreymonSkill2_Part2*>(Find_PartObject(TEXT("Part_Skill2")));
	return S_OK;
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