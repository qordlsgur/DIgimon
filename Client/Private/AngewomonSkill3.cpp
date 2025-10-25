#include "AngewomonSkill3.h"
#include "GameInstance.h"
#include "Interaction_Manager.h"
#include "AngewomonSkill3_Part1.h"
#include "AngewomonSkill3_Part2.h"

CAngewomonSkill3::CAngewomonSkill3(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSkillObject{ pDevice, pContext }
{
}

CAngewomonSkill3::CAngewomonSkill3(const CAngewomonSkill3& Prototype)
	: CSkillObject{ Prototype }
{
}

HRESULT CAngewomonSkill3::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAngewomonSkill3::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC	Desc{};
	Desc.fRotationPerSec = XMConvertToRadians(180.0f);
	Desc.fSpeedPerSec = 300.f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	m_pInteraction_Manager = CInteraction_Manager::GetInstance();
	m_pInteraction_Manager->Set_Attack_Digimon(this);


	const POSITION* Pos = static_cast<const POSITION*>(pArg);

	m_iDamage = Pos->iDamage;

	m_vPosition = Pos->m_vPosition;
	m_vFirstPosigion = Pos->m_vPosition;
	m_iDamage = Pos->iDamage;
	m_vTarget_pos = Pos->m_vTargetPosition;

	m_vTarget_pos.m128_f32[1] += 10.f;

	m_vFirstPosigion.m128_f32[1] += 12.f;
	m_vPosition.m128_f32[1] += 10.f;

	if (Pos->Look == 0)
	{
		m_vFirstPosigion.m128_f32[0] -= 1.f;
		m_vFirstPosigion.m128_f32[2] += 3.f;
	}
	else
	{
		m_vFirstPosigion.m128_f32[0] += 1.f;
	}

	m_pTransformCom->Set_State(STATE::POSITION, m_vPosition);
	m_pTransformCom->Update_WoldMatrix();
	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	if (FAILED(Ready_SkillObjects()))
		return E_FAIL;



	return S_OK;
}

void CAngewomonSkill3::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CAngewomonSkill3::Update(_float fTimeDelta)
{
	_fTime += fTimeDelta;

	m_pTransformCom->TargetLook(m_vTarget_pos);

	if (_fTime >= 1.4f)
		m_pTransformCom->Target_Pos_Move(m_vTarget_pos, fTimeDelta);
	else
		m_pTransformCom->Set_State(STATE::POSITION, m_vFirstPosigion);

	m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));

	__super::Update(fTimeDelta);
}

void CAngewomonSkill3::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::UI, this);
	__super::Late_Update(fTimeDelta);
}

HRESULT CAngewomonSkill3::Render()
{
#ifdef _DEBUG
	m_pColliderCom->Render();
#endif

	return S_OK;
}

_int CAngewomonSkill3::Get_Damage()
{
	return m_iDamage;
}

HRESULT CAngewomonSkill3::Ready_PartObjects()
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

HRESULT CAngewomonSkill3::Ready_SkillObjects()
{
	CAngewomonSkill3_Part1::BODY_PLAYER_DESC Skill1{};

	Skill1.pParentTransform = m_pTransformCom;

	/* Part_Skill1 */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_AngewomonSkill3_Part1"),
		TEXT("Part_Skill1"), &Skill1)))
		return E_FAIL;

	m_pSkillModel1 = dynamic_cast<CAngewomonSkill3_Part1*>(Find_PartObject(TEXT("Part_Skill1")));

	//CAngewomonSkill3_Part2::BODY_PLAYER_DESC Skill2{};
	//Skill2.pParentTransform = m_pTransformCom;

	///* Part_Skill2 */
	//if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_AngewomonSkill3_Part2"),
	//	TEXT("Part_Skill2"), &Skill2)))
	//	return E_FAIL;

	//m_pSkillModel2 = dynamic_cast<CAngewomonSkill3_Part2*>(Find_PartObject(TEXT("Part_Skill2")));

	return S_OK;
}

CAngewomonSkill3* CAngewomonSkill3::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CAngewomonSkill3* pInstance = new CAngewomonSkill3(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CAngewomonSkill3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CAngewomonSkill3::Clone(void* pArg)
{
	CAngewomonSkill3* pInstance = new CAngewomonSkill3(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CAngewomonSkill3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAngewomonSkill3::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);

}
