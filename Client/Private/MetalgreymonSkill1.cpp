#include "MetalgreymonSkill1.h"
#include "GameInstance.h"
#include "Interaction_Manager.h"
#include "MetalgreymonSkill1_Part1.h"
#include "MetalgreymonSkill1_Part2.h"
#include "MetalgreymonSkill1_Part3.h"

CMetalgreymonSkill1::CMetalgreymonSkill1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSkillObject{ pDevice, pContext }
{
}

CMetalgreymonSkill1::CMetalgreymonSkill1(const CMetalgreymonSkill1& Prototype)
	: CSkillObject{ Prototype }
{
}

HRESULT CMetalgreymonSkill1::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMetalgreymonSkill1::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC	Desc{};
	Desc.fRotationPerSec = XMConvertToRadians(180.0f);
	Desc.fSpeedPerSec = 10.f;

	m_fFontUp = 10.f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;


	m_pInteraction_Manager = CInteraction_Manager::GetInstance();
	m_pInteraction_Manager->Set_Attack_Digimon(this);


	const POSITION* Pos = static_cast<const POSITION*>(pArg);

	m_iDamage = Pos->iDamage;
	m_vPosition = Pos->m_vPosition2;
	m_vTarget_pos = Pos->m_vTargetPosition;
	if (Pos->Look == 0)
	{
		m_vPosition.m128_f32[2] -= 5.f;
		m_vTarget_pos.m128_f32[2] -= 5.f;

	}
	else
	{
		m_vPosition.m128_f32[2] += 3.f;
		m_vTarget_pos.m128_f32[2] += 3.f;

	}
	m_vPosition.m128_f32[1] += 20.f;
	m_vTarget_pos.m128_f32[1] += 10.f;
	m_pTransformCom->Set_State(STATE::POSITION, m_vPosition);
	m_pTransformCom->Update_WoldMatrix();
	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	if (FAILED(Ready_SkillObjects()))
		return E_FAIL;
	if (FAILED(Ready_Skill2Objects()))
		return E_FAIL;


	return S_OK;
}

void CMetalgreymonSkill1::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CMetalgreymonSkill1::Update(_float fTimeDelta)
{
	if (m_bMove)
		if (!m_pTransformCom->Target_Pos_Move_Bool(m_vTarget_pos, fTimeDelta * 15.f))
		{
			Ready_Skill3Objects();
			m_pSkillModel1->Set_Hit(true);
			m_pSkillModel2->Set_Hit(true);
			m_fTime += fTimeDelta;
			m_fFontUp += fTimeDelta * 2.f;
		}

	if (m_fTime >= 2.f)
	{
		m_pInteraction_Manager->Die_Attack_Skill();
		m_isDead = true;
	}

	

	m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));

	__super::Update(fTimeDelta);
}


void CMetalgreymonSkill1::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::EFFECT, this);
	__super::Late_Update(fTimeDelta);
}

HRESULT CMetalgreymonSkill1::Render()
{
	if (m_bHit)
	{
		_itow_s(m_iDamage, m_szDamage, MAX_PATH, 10);

		m_pGameInstance->Perspective_Render_Text(
			m_pGameInstance->Get_Transform_Matrix(D3DTS::VIEW),
			m_pGameInstance->Get_Transform_Matrix(D3DTS::PROJ),
			TEXT("42"), m_szDamage,

		XMVectorSet(m_vTarget_pos.m128_f32[0],
			m_vTarget_pos.m128_f32[1] + m_fFontUp,
			m_vTarget_pos.m128_f32[2],
			m_vTarget_pos.m128_f32[3]));
	}

	return S_OK;
}

_int CMetalgreymonSkill1::Get_Damage()
{
	return m_iDamage;
}
HRESULT CMetalgreymonSkill1::Ready_PartObjects()
{
	/* Com_Sphere*/
	CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};
	SphereDesc.fRadius = 2.f;
	SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius, 0.f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider_Sphere"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
		return E_FAIL;

	m_pInteraction_Manager->Set_Skill_Collider(m_pColliderCom);
	m_pColliderCom->Set_Matrix(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));

	return S_OK;;
}

HRESULT CMetalgreymonSkill1::Ready_SkillObjects()
{
	CMetalgreymonSkill1_Part1::BODY_PLAYER_DESC Skill1{};

	Skill1.pParentTransform = m_pTransformCom;

	/* Part_Skill1 */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgreymonSkill1_Part1"),
		TEXT("Part_Skill1"), &Skill1)))
		return E_FAIL;

	m_pSkillModel1 = dynamic_cast<CMetalgreymonSkill1_Part1*>(Find_PartObject(TEXT("Part_Skill1")));
	return S_OK;
}

HRESULT CMetalgreymonSkill1::Ready_Skill2Objects()
{
	CMetalgreymonSkill1_Part2::BODY_PLAYER_DESC Skill2{};

	Skill2.pParentTransform = m_pTransformCom;
	Skill2.vPosition = m_vTarget_pos;
	/* Part_Skill1 */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgreymonSkill1_Part2"),
		TEXT("Part_Skill2"), &Skill2)))
		return E_FAIL;

	m_pSkillModel2 = dynamic_cast<CMetalgreymonSkill1_Part2*>(Find_PartObject(TEXT("Part_Skill2")));

	return S_OK;
}

HRESULT CMetalgreymonSkill1::Ready_Skill3Objects()
{
	CMetalgreymonSkill1_Part3::BODY_PLAYER_DESC Skill3{};

	Skill3.pParentTransform = m_pTransformCom;
	Skill3.vPosition = m_vTarget_pos;

	/* Part_Skill3 */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgreymonSkill1_Part3"),
		TEXT("Part_Skill3"), &Skill3)))
		return E_FAIL;

	m_pSkillModel3 = dynamic_cast<CMetalgreymonSkill1_Part3*>(Find_PartObject(TEXT("Part_Skill3")));


	return S_OK;
}

CMetalgreymonSkill1* CMetalgreymonSkill1::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMetalgreymonSkill1* pInstance = new CMetalgreymonSkill1(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMetalgreymonSkill1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMetalgreymonSkill1::Clone(void* pArg)
{
	CMetalgreymonSkill1* pInstance = new CMetalgreymonSkill1(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMetalgreymonSkill1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMetalgreymonSkill1::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);

}