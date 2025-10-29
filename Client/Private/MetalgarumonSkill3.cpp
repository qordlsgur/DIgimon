#include "MetalgarumonSkill3.h"
#include "GameInstance.h"
#include "Interaction_Manager.h"
#include "MetalgarumonSkill3_Part1.h"

CMetalgarumonSkill3::CMetalgarumonSkill3(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CSkillObject{ pDevice, pContext }
{
}

CMetalgarumonSkill3::CMetalgarumonSkill3(const CMetalgarumonSkill3& Prototype)
    : CSkillObject{ Prototype }
{
}

HRESULT CMetalgarumonSkill3::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMetalgarumonSkill3::Initialize(void* pArg)
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
	m_pTransformCom->Set_Scale(3.f, 3.f, 10.f);


	m_pTransformCom->Set_State(STATE::POSITION, m_vPosition);
	m_pTransformCom->Update_WoldMatrix();
	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	if (FAILED(Ready_SkillObjects()))
		return E_FAIL;

	return S_OK;
}

void CMetalgarumonSkill3::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CMetalgarumonSkill3::Update(_float fTimeDelta)
{
	m_pTransformCom->TargetLook(m_vTarget_pos);

	m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));

	__super::Update(fTimeDelta);
}


void CMetalgarumonSkill3::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::EFFECT, this);
	__super::Late_Update(fTimeDelta);
}

HRESULT CMetalgarumonSkill3::Render()
{
#ifdef _DEBUG
	m_pColliderCom->Render();
#endif

	return S_OK;
}

_int CMetalgarumonSkill3::Get_Damage()
{
	return m_iDamage;
}

HRESULT CMetalgarumonSkill3::Ready_PartObjects()
{
	/* Com_Sphere*/
	CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};
	SphereDesc.fRadius = 5.f;
	SphereDesc.vCenter = _float3(0.f, 0.f, SphereDesc.fRadius);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider_Sphere"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
		return E_FAIL;

	m_pInteraction_Manager->Set_Skill_Collider(m_pColliderCom);
	m_pColliderCom->Set_Matrix(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));

	return S_OK;
}

HRESULT CMetalgarumonSkill3::Ready_SkillObjects()
{
	CMetalgarumonSkill3_Part1::BODY_PLAYER_DESC Skill1{};

	Skill1.pParentTransform = m_pTransformCom;

	/* Part_Skill1 */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgarumonSkill3_Part1"),
		TEXT("Part_Skill1"), &Skill1)))
		return E_FAIL;

	m_pSkillModel1 = dynamic_cast<CMetalgarumonSkill3_Part1*>(Find_PartObject(TEXT("Part_Skill1")));
	static_cast<CMetalgarumonSkill3_Part1*>(m_pSkillModel1)->Set_Mouth(m_mMouth);
	return S_OK;
}

CMetalgarumonSkill3* CMetalgarumonSkill3::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMetalgarumonSkill3* pInstance = new CMetalgarumonSkill3(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMetalgarumonSkill3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMetalgarumonSkill3::Clone(void* pArg)
{
	CMetalgarumonSkill3* pInstance = new CMetalgarumonSkill3(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMetalgarumonSkill3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMetalgarumonSkill3::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);

}