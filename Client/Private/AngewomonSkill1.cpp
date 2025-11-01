#include "AngewomonSkill1.h"
#include "GameInstance.h"
#include "Interaction_Manager.h"
#include "AngewomonSkill1_Part1.h"
#include "AngewomonSkill1_Part2.h"

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


	m_pInteraction_Manager = CInteraction_Manager::GetInstance();
	m_pInteraction_Manager->Set_Attack_Digimon(this);


	const POSITION* Pos = static_cast<const POSITION*>(pArg);

	m_iDamage = Pos->iDamage;
	m_vPosition = Pos->m_vPosition;
	m_vTarget_pos = Pos->m_vTargetPosition;

	m_pTransformCom->Set_State(STATE::POSITION, m_vPosition);
	m_fDir = Pos->Look;
	if (m_fDir == 0)
	{
		m_vTarget_pos.m128_f32[2] += 5.f;
		m_vTarget_pos.m128_f32[1] += 10.f;
		m_pTransformCom->Set_Scale(-2.0f, 2.0f, -2.0f);
		m_pTransformCom->Update_WoldMatrix();
	}
	else
	{
		m_vTarget_pos.m128_f32[2] -= 5.f;
		m_vTarget_pos.m128_f32[1] += 10.f;
		m_pTransformCom->Set_Scale(2.f, 2.f, 2.f);
		m_pTransformCom->Update_WoldMatrix();
	}

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	if (FAILED(Ready_SkillObjects()))
		return E_FAIL;

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

	if (m_bHit == true)
	{
		m_fTime += fTimeDelta;
		m_fFontUp += fTimeDelta;
	}

	if (m_fTime > 1.f)
	{
		m_pInteraction_Manager->Die_Attack_Skill();
		m_isDead = true;
	}

	__super::Update(fTimeDelta);
}

void CAngewomonSkill1::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::EFFECT, this);
	__super::Late_Update(fTimeDelta);
}

HRESULT CAngewomonSkill1::Render()
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

	m_pInteraction_Manager->Set_Skill_Collider(m_pColliderCom);
	m_pColliderCom->Set_Matrix(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));

	return S_OK;;
}

HRESULT CAngewomonSkill1::Ready_SkillObjects()
{
	CAngewomonSkill1_Part1::BODY_PLAYER_DESC Skill1{};

	Skill1.pParentTransform = m_pTransformCom;
	Skill1.vPosition = m_pTransformCom->Get_State(STATE::POSITION);
	Skill1.LR = m_fDir;
	/* Part_Skill1 */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_AngewomonSkill1_Part1"),
		TEXT("Part_Skill1"), &Skill1)))
		return E_FAIL;

	m_pSkillModel1 = dynamic_cast<CAngewomonSkill1_Part1*>(Find_PartObject(TEXT("Part_Skill1")));

	return S_OK;
}

HRESULT CAngewomonSkill1::Ready_SkillObjects2()
{
	//CAngewomonSkill1_Part2::BODY_PLAYER_DESC Skill1{};

	//Skill1.vPosition = m_pTransformCom->Get_State(STATE::POSITION);

	///* Part_Skill1 */
	//if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_AngewomonSkill1_Part2"),
	//	TEXT("Part_Skill2"), &Skill1)))
	//	return E_FAIL;

	//m_pSkillModel1 = dynamic_cast<CAngewomonSkill1_Part1*>(Find_PartObject(TEXT("Part_Skill2")));

	return S_OK;
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