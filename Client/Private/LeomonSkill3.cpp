#include "LeomonSkill3.h"
#include "GameInstance.h"
#include "Interaction_Manager.h"
#include "LeomonSkill3_Part1.h"

CLeomonSkill3::CLeomonSkill3(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSkillObject{ pDevice, pContext }
{
}

CLeomonSkill3::CLeomonSkill3(const CLeomonSkill3& Prototype)
	: CSkillObject{ Prototype }
{
}

HRESULT CLeomonSkill3::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLeomonSkill3::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC	Desc{};
	Desc.fRotationPerSec = XMConvertToRadians(180.0f);
	Desc.fSpeedPerSec = 10.f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	m_pInteraction_Manager = CInteraction_Manager::GetInstance();
	m_pInteraction_Manager->Set_Attack_Digimon(this);
	m_pInteraction_Manager->Set_Skill_Collider(m_pColliderCom);

	const POSITION* Pos = static_cast<const POSITION*>(pArg);

	m_iDamage = Pos->iDamage;

	m_vTarget_pos = Pos->m_vTargetPosition;
	m_vTarget_pos.m128_f32[1] += 2.f;

	m_pTransformCom->Set_State(STATE::POSITION, m_vTarget_pos);
	m_pTransformCom->Update_WoldMatrix();

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	return S_OK;
}

void CLeomonSkill3::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CLeomonSkill3::Update(_float fTimeDelta)
{
	if (m_bHit)
	{
		Ready_SkillObjects();
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

void CLeomonSkill3::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::EFFECT, this);
	__super::Late_Update(fTimeDelta);
}

HRESULT CLeomonSkill3::Render()
{
	if (m_bHit)
	{
		_itow_s(m_iDamage, m_szDamage, MAX_PATH, 10);

		m_pGameInstance->Perspective_Render_Text(
			m_pGameInstance->Get_Transform_Matrix(D3DTS::VIEW),
			m_pGameInstance->Get_Transform_Matrix(D3DTS::PROJ),
			TEXT("42"), m_szDamage,

			XMVectorSet(m_vTarget_pos.m128_f32[0],
				m_vTarget_pos.m128_f32[1] + m_fFontUp + 20.f,
				m_vTarget_pos.m128_f32[2],
				m_vTarget_pos.m128_f32[3]));
	}
	return S_OK;
}

void CLeomonSkill3::Set_Pos(_float fX, _float fY)
{
}

_int CLeomonSkill3::Get_Damage()
{
	return m_iDamage;
}

HRESULT CLeomonSkill3::Ready_PartObjects()
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

	return S_OK;
}
HRESULT CLeomonSkill3::Ready_SkillObjects()
{
	CLeomonSkill3_Part1::BODY_PLAYER_DESC Skill1{};

	Skill1.pParentTransform = m_pTransformCom;

	/* Part_Skill1 */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_LeomonSkill3_Part1"),
		TEXT("Part_Skill1"), &Skill1)))
		return E_FAIL;

	m_pSkillModel1 = dynamic_cast<CLeomonSkill3_Part1*>(Find_PartObject(TEXT("Part_Skill1")));

	return S_OK;
}

CLeomonSkill3* CLeomonSkill3::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLeomonSkill3* pInstance = new CLeomonSkill3(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLeomonSkill3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLeomonSkill3::Clone(void* pArg)
{
	CLeomonSkill3* pInstance = new CLeomonSkill3(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CLeomonSkill3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLeomonSkill3::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);

}
