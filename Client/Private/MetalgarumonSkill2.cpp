#include "MetalgarumonSkill2.h"
#include "GameInstance.h"
#include "Interaction_Manager.h"
#include "MetalgarumonSkill2_Part1.h"
#include "MetalgarumonSkill2_Part2.h"
#include "MetalgarumonSkill2_Part3.h"
#include "MetalgarumonSkill2_Part4.h"
#include "MetalgarumonSkill2_Part5.h"

CMetalgarumonSkill2::CMetalgarumonSkill2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CSkillObject{ pDevice, pContext }
{
}

CMetalgarumonSkill2::CMetalgarumonSkill2(const CMetalgarumonSkill2& Prototype)
    : CSkillObject{ Prototype }
{
}

HRESULT CMetalgarumonSkill2::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMetalgarumonSkill2::Initialize(void* pArg)
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
	//m_vTarget_pos = XMVectorSet(100.f, 0.f, 300.f, 1.f);
	m_pTransformCom->Set_State(STATE::POSITION, m_vPosition);

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	if (FAILED(Ready_SkillObjects()))
		return E_FAIL;

	return S_OK;
}

void CMetalgarumonSkill2::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}			


void CMetalgarumonSkill2::Update(_float fTimeDelta)
{

	if (!m_pTransformCom->Target_Pos_Move_Bool(m_vTarget_pos, fTimeDelta))
	{
		m_pSkillModel1->Set_Hit(true);
		Ready_SkillObjects2();
		m_fTime += fTimeDelta;
		m_fTime += fTimeDelta;
		m_fFontUp += fTimeDelta * 2.f;
	}

	if (m_fTime > 2.f)
	{
		m_pInteraction_Manager->Die_Attack_Skill();
		m_isDead = true;
	}

	m_pTransformCom->TargetLook(m_vTarget_pos);
	m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));

	__super::Update(fTimeDelta);
}

void CMetalgarumonSkill2::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::EFFECT, this);
	__super::Late_Update(fTimeDelta);
}

HRESULT CMetalgarumonSkill2::Render()
{
	if (m_bHit)
	{
		_itow_s(m_iDamage, m_szDamage, MAX_PATH, 10);

		m_pGameInstance->Perspective_Render_Text(
			m_pGameInstance->Get_Transform_Matrix(D3DTS::VIEW),
			m_pGameInstance->Get_Transform_Matrix(D3DTS::PROJ),
			TEXT("42"), m_szDamage,

			XMVectorSet(m_vTarget_pos.m128_f32[0],
				m_vTarget_pos.m128_f32[1] + m_fFontUp + 10.f,
				m_vTarget_pos.m128_f32[2],
				m_vTarget_pos.m128_f32[3]));
	}
	return S_OK;
}

_int CMetalgarumonSkill2::Get_Damage()
{
	return m_iDamage;
}

HRESULT CMetalgarumonSkill2::Ready_PartObjects()
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

HRESULT CMetalgarumonSkill2::Ready_SkillObjects()
{
	CMetalgarumonSkill2_Part1::BODY_PLAYER_DESC Skill1{};

	Skill1.pParentTransform = m_pTransformCom;

	/* Part_Skill1 */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgarumonSkill2_Part1"),
		TEXT("Part_Skill1"), &Skill1)))
		return E_FAIL;

	m_pSkillModel1 = dynamic_cast<CMetalgarumonSkill2_Part1*>(Find_PartObject(TEXT("Part_Skill1")));

	CMetalgarumonSkill2_Part2::BODY_PLAYER_DESC Skill2{};

	Skill2.pParentTransform = m_pTransformCom;
	Skill2.vPosition = m_vTarget_pos;

	/* Part_Skill2 */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgarumonSkill2_Part2"),
		TEXT("Part_Skill2"), &Skill2)))
		return E_FAIL;

	m_pSkillModel2 = dynamic_cast<CMetalgarumonSkill2_Part2*>(Find_PartObject(TEXT("Part_Skill2")));

	CMetalgarumonSkill2_Part3::BODY_PLAYER_DESC Skill3{};

	Skill3.pParentTransform = m_pTransformCom;
	Skill3.vPosition = m_vTarget_pos;

	/* Part_Skill3 */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgarumonSkill2_Part3"),
		TEXT("Part_Skill3"), &Skill3)))
		return E_FAIL;

	m_pSkillModel3 = dynamic_cast<CMetalgarumonSkill2_Part3*>(Find_PartObject(TEXT("Part_Skill3")));
	return S_OK;
}

HRESULT CMetalgarumonSkill2::Ready_SkillObjects2()
{
	CMetalgarumonSkill2_Part4::BODY_PLAYER_DESC Skil14{};

	Skil14.pParentTransform = m_pTransformCom;
	Skil14.vPosition = m_vTarget_pos;

	/* Part_Skill2 */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgarumonSkill2_Part4"),
		TEXT("Part_Skill4"), &Skil14)))
		return E_FAIL;

	m_pSkillModel4 = dynamic_cast<CMetalgarumonSkill2_Part4*>(Find_PartObject(TEXT("Part_Skill4")));

	CMetalgarumonSkill2_Part5::BODY_PLAYER_DESC Skil15{};

	Skil15.pParentTransform = m_pTransformCom;
	Skil15.vPosition = m_vTarget_pos;

	/* Part_Skill2 */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgarumonSkill2_Part5"),
		TEXT("Part_Skill5"), &Skil15)))
		return E_FAIL;

	m_pSkillModel5 = dynamic_cast<CMetalgarumonSkill2_Part5*>(Find_PartObject(TEXT("Part_Skill5")));
	return S_OK;
}

CMetalgarumonSkill2* CMetalgarumonSkill2::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMetalgarumonSkill2* pInstance = new CMetalgarumonSkill2(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMetalgarumonSkill2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMetalgarumonSkill2::Clone(void* pArg)
{
	CMetalgarumonSkill2* pInstance = new CMetalgarumonSkill2(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMetalgarumonSkill2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMetalgarumonSkill2::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);

}