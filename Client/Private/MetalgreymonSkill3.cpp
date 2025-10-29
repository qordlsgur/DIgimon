#include "MetalgreymonSkill3.h"
#include "GameInstance.h"
#include "Interaction_Manager.h"
#include "MetalgreymonSkill3_Part1.h"
#include "MetalgreymonSkill3_Part2.h"

CMetalgreymonSkill3::CMetalgreymonSkill3(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CSkillObject{ pDevice, pContext }
{
}

CMetalgreymonSkill3::CMetalgreymonSkill3(const CMetalgreymonSkill3& Prototype)
    : CSkillObject{ Prototype }
{
}

HRESULT CMetalgreymonSkill3::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMetalgreymonSkill3::Initialize(void* pArg)
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
	m_vPosition2 = Pos->m_vPosition;
	m_vTarget_pos = Pos->m_vTargetPosition;
	m_pTransformCom->Update_WoldMatrix();

	m_mLeftHatch = Pos->mMatrix;
	m_mRightHatch = Pos->mMatrix2;
	m_pTransformCom->Set_Scale(1.f, 1.f, 10.f);

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	if (FAILED(Ready_SkillObjects2()))
		return E_FAIL;
	m_pTransformCom->Set_State(STATE::POSITION, m_vPosition);

	return S_OK;
}

void CMetalgreymonSkill3::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CMetalgreymonSkill3::Update(_float fTimeDelta)
{
	m_pTransformCom->TargetLook(m_vTarget_pos);

	m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));

	if (m_bMove == true)
		Ready_SkillObjects();

	__super::Update(fTimeDelta);
}

void CMetalgreymonSkill3::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::EFFECT, this);
	__super::Late_Update(fTimeDelta);
}

HRESULT CMetalgreymonSkill3::Render()
{
#ifdef _DEBUG
	m_pColliderCom->Render();
#endif

	return S_OK;
}

_int CMetalgreymonSkill3::Get_Damage()
{
	return m_iDamage;
}


HRESULT CMetalgreymonSkill3::Ready_PartObjects()
{
	/* Com_Sphere*/
	CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};
	SphereDesc.fRadius = 10.f;
	SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius, 0.f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider_Sphere"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
		return E_FAIL;

	m_pInteraction_Manager->Set_Skill_Collider(m_pColliderCom);
	m_pColliderCom->Set_Matrix(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));

	return S_OK;;
}

HRESULT CMetalgreymonSkill3::Ready_SkillObjects()
{
	CMetalgreymonSkill3_Part1::BODY_PLAYER_DESC Skill1{};

	Skill1.pParentTransform = m_pTransformCom;
	Skill1.vMatrix = m_mLeftHatch;
	Skill1.LR = 3.2f;
	/* Part_Skill1 */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgreymonSkill3_Part1"),
		TEXT("Part_Skill1"), &Skill1)))
		return E_FAIL;

	m_pSkillModel1 = dynamic_cast<CMetalgreymonSkill3_Part1*>(Find_PartObject(TEXT("Part_Skill1")));

	CMetalgreymonSkill3_Part1::BODY_PLAYER_DESC Skill2{};

	Skill2.pParentTransform = m_pTransformCom;
	Skill2.vMatrix = m_mRightHatch;
	Skill2.LR = 1.2f;
	/* Part_Skill2 */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgreymonSkill3_Part1"),
		TEXT("Part_Skill2"), &Skill2)))
		return E_FAIL;

	m_pSkillModel2 = dynamic_cast<CMetalgreymonSkill3_Part1*>(Find_PartObject(TEXT("Part_Skill2")));


	return S_OK;
}

HRESULT CMetalgreymonSkill3::Ready_SkillObjects2()
{
	CMetalgreymonSkill3_Part2::BODY_PLAYER_DESC Skill3{};


	Skill3.pParentTransform = m_pTransformCom;
	/* Part_Skill3 */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgreymonSkill3_Part2"),
		TEXT("Part_Skill3"), &Skill3)))
		return E_FAIL;

	m_pSkillModel3 = dynamic_cast<CMetalgreymonSkill3_Part2*>(Find_PartObject(TEXT("Part_Skill3")));

	return S_OK;
}

CMetalgreymonSkill3* CMetalgreymonSkill3::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMetalgreymonSkill3* pInstance = new CMetalgreymonSkill3(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMetalgreymonSkill3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMetalgreymonSkill3::Clone(void* pArg)
{
	CMetalgreymonSkill3* pInstance = new CMetalgreymonSkill3(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMetalgreymonSkill3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMetalgreymonSkill3::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);

}