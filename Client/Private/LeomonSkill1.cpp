#include "LeomonSkill1.h"
#include "GameInstance.h"
#include "Interaction_Manager.h"
#include "LeomonSkill1_Part1.h"
#include "LeomonSkill1_Part2.h"

CLeomonSkill1::CLeomonSkill1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSkillObject{ pDevice, pContext }
{
}

CLeomonSkill1::CLeomonSkill1(const CLeomonSkill1& Prototype)
	: CSkillObject{ Prototype }
{
}

HRESULT CLeomonSkill1::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLeomonSkill1::Initialize(void* pArg)
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
	if (Pos->Look == 1)
	{
		m_vTarget_pos.m128_f32[2] += 3.f;
		m_pTransformCom->Set_Scale(10.f, -10.f, 10.f);
		m_pTransformCom->Update_WoldMatrix();
	}
	else
	{
		m_vTarget_pos.m128_f32[2] -= 3.f;
		m_pTransformCom->Set_Scale(-10.f, -10.f, -10.f);
		m_pTransformCom->Update_WoldMatrix();
	}
	m_vTarget_pos.m128_f32[1] += 10.f;
	m_pTransformCom->Set_State(STATE::POSITION, m_vTarget_pos);
	m_fEndTime = 3.f;
	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	if (FAILED(Ready_SkillObjects()))
		return E_FAIL;
	return S_OK;
}

void CLeomonSkill1::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CLeomonSkill1::Update(_float fTimeDelta)
{
	m_fTime = fTimeDelta;

	if (m_bMove)
	{
		m_pSkillModel1->Set_Hit(true);
		Ready_SkillObjects2();
	}

	if (m_bHit == true && m_iHitCount <= 2)
	{
		if (iHitCount != m_iHitCount)
		{
			HITINFO Info;

			Info.damage = m_iDamage;
			Info.pos =
				XMVectorSet(m_vTarget_pos.m128_f32[0],
					m_vTarget_pos.m128_f32[1] + m_fFontUp + 10.f,
					m_vTarget_pos.m128_f32[2],
					m_vTarget_pos.m128_f32[3]);
			Info.duration = 1.f;
			m_Info.push_back(Info);
			iHitCount = m_iHitCount;
		}
	}

	if (iHitCount == 2)
	{
		m_fEndTime -= fTimeDelta * 3.f;
	}

	if (m_fEndTime <= 0.f)
	{
		m_pInteraction_Manager->Die_Attack_Skill();
		iHitCount = m_iHitCount = 0;
		m_fEndTime = 0.f;
		m_isDead = true;
	}



	m_pTransformCom->Set_State((STATE::POSITION), m_vTarget_pos);

	m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));

	__super::Update(fTimeDelta);
}

void CLeomonSkill1::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::EFFECT, this);
	__super::Late_Update(fTimeDelta);
}

HRESULT CLeomonSkill1::Render()
{
	for (auto it = m_Info.begin(); it != m_Info.end();)
	{
		wchar_t szDamage[MAX_PATH];
		_itow_s(it->damage, szDamage, MAX_PATH, 10);

		m_pGameInstance->Perspective_Render_Text(
			m_pGameInstance->Get_Transform_Matrix(D3DTS::VIEW),
			m_pGameInstance->Get_Transform_Matrix(D3DTS::PROJ),
			TEXT("42"), szDamage, it->pos);

		_vector up = XMVectorSet(0.f, 1.f * (1.f - it->duration) * 50.f, 0.f, 0.f);
		it->pos = XMVectorAdd(it->pos, up * m_fTime * 0.5f);

		// 시간 감소
		it->duration -= m_fTime;
		if (it->duration <= 0.f)
			it = m_Info.erase(it);
		else
			++it;
	}

	return S_OK;
}

void CLeomonSkill1::Set_Pos(_float fX, _float fY)
{

}

_int CLeomonSkill1::Get_Damage()
{
	return m_iDamage;
}

HRESULT CLeomonSkill1::Ready_PartObjects()
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

HRESULT CLeomonSkill1::Ready_SkillObjects()
{
	CLeomonSkill1_Part1::BODY_PLAYER_DESC Skill1{};

	Skill1.pParentTransform = m_pTransformCom;
	Skill1.vPosition = m_vTarget_pos;
	/* Part_Skill1 */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_LeomonSkill1_Part1"),
		TEXT("Part_Skill1"), &Skill1)))
		return E_FAIL;

	m_pSkillModel1 = dynamic_cast<CLeomonSkill1_Part1*>(Find_PartObject(TEXT("Part_Skill1")));

	return S_OK;
}

HRESULT CLeomonSkill1::Ready_SkillObjects2()
{
	CLeomonSkill1_Part2::BODY_PLAYER_DESC Skill2{};

	Skill2.pParentTransform = m_pTransformCom;
	Skill2.vPosition = m_vPosition2;

	/* Part_Skill1 */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_LeomonSkill1_Part2"),
		TEXT("Part_Skill2"), &Skill2)))
		return E_FAIL;

	m_pSkillModel2 = dynamic_cast<CLeomonSkill1_Part2*>(Find_PartObject(TEXT("Part_Skill2")));

	return S_OK;
}

CLeomonSkill1* CLeomonSkill1::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLeomonSkill1* pInstance = new CLeomonSkill1(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLeomonSkill1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLeomonSkill1::Clone(void* pArg)
{
	CLeomonSkill1* pInstance = new CLeomonSkill1(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CLeomonSkill1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLeomonSkill1::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);

}

