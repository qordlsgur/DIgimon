#include "MetalgarumonSkill1.h"
#include "GameInstance.h"
#include "Interaction_Manager.h"
#include "MetalgarumonSkill1_Part1.h"

CMetalgarumonSkill1::CMetalgarumonSkill1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CSkillObject{ pDevice, pContext }
{
}

CMetalgarumonSkill1::CMetalgarumonSkill1(const CMetalgarumonSkill1& Prototype)
    : CSkillObject{ Prototype }
{
}

HRESULT CMetalgarumonSkill1::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMetalgarumonSkill1::Initialize(void* pArg)
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

	m_vTarget_pos = Pos->m_vPosition2;

	m_pTransformCom->Set_State(STATE::POSITION, m_vTarget_pos);
	m_pTransformCom->Update_WoldMatrix();

	m_fDir = Pos->Look;
	if (m_fDir == 0)
	{
		m_vTarget_pos.m128_f32[2] += 3.f;
		m_pTransformCom->Set_Scale(20.f, -20.f, 20.f);
		m_pTransformCom->Update_WoldMatrix();
	}
	else
	{
		m_vTarget_pos.m128_f32[2] -= 3.f;
		m_pTransformCom->Set_Scale(-20.f, -20.f, -20.f);
		m_pTransformCom->Update_WoldMatrix();
	}
	m_vTarget_pos.m128_f32[1] += 3.f;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	if (FAILED(Ready_SkillObjects()))
		return E_FAIL;

	return S_OK;
}

void CMetalgarumonSkill1::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CMetalgarumonSkill1::Update(_float fTimeDelta)
{

	if (m_bHit)
	{
		m_pSkillModel1->Set_Hit(true);
		m_fTime += fTimeDelta;
		m_fFontUp += fTimeDelta * 2.f;
	}


	if (m_fTime > 2.f)
	{
		m_pInteraction_Manager->Die_Attack_Skill();
		m_isDead = true;
	}

	m_pTransformCom->Set_State((STATE::POSITION), m_vTarget_pos);

	m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));

	__super::Update(fTimeDelta);
}

void CMetalgarumonSkill1::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::EFFECT, this);
	__super::Late_Update(fTimeDelta);
}

HRESULT CMetalgarumonSkill1::Render()
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

_int CMetalgarumonSkill1::Get_Damage()
{
	return m_iDamage;
}

HRESULT CMetalgarumonSkill1::Ready_PartObjects()
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

HRESULT CMetalgarumonSkill1::Ready_SkillObjects()
{
	CMetalgarumonSkill1_Part1::BODY_PLAYER_DESC Skill1{};

	Skill1.pParentTransform = m_pTransformCom;

	/* Part_Skill1 */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgarumonSkill1_Part1"),
		TEXT("Part_Skill1"), &Skill1)))
		return E_FAIL;

	m_pSkillModel1 = dynamic_cast<CMetalgarumonSkill1_Part1*>(Find_PartObject(TEXT("Part_Skill1")));
	CMetalgarumonSkill1_Part1::BODY_PLAYER_DESC Skill2{};

	Skill2.pParentTransform = m_pTransformCom;

	/* Part_Skill1 */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgarumonSkill1_Part1"),
		TEXT("Part_Skill2"), &Skill2)))
		return E_FAIL;

	m_pSkillModel1 = dynamic_cast<CMetalgarumonSkill1_Part1*>(Find_PartObject(TEXT("Part_Skill2")));
	CMetalgarumonSkill1_Part1::BODY_PLAYER_DESC Skill3{};

	Skill3.pParentTransform = m_pTransformCom;

	/* Part_Skill1 */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgarumonSkill1_Part1"),
		TEXT("Part_Skill3"), &Skill3)))
		return E_FAIL;

	m_pSkillModel1 = dynamic_cast<CMetalgarumonSkill1_Part1*>(Find_PartObject(TEXT("Part_Skill3")));
	return S_OK;
}

CMetalgarumonSkill1* CMetalgarumonSkill1::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMetalgarumonSkill1* pInstance = new CMetalgarumonSkill1(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMetalgarumonSkill1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMetalgarumonSkill1::Clone(void* pArg)
{
	CMetalgarumonSkill1* pInstance = new CMetalgarumonSkill1(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMetalgarumonSkill1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMetalgarumonSkill1::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);

}