#include "BlackwargreymonSkill3.h"
#include "GameInstance.h"
#include "Interaction_Manager.h"
#include "BlackwargreymonSkill3_Part1.h"

CBlackwargreymonSkill3::CBlackwargreymonSkill3(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSkillObject{ pDevice, pContext }
{
}

CBlackwargreymonSkill3::CBlackwargreymonSkill3(const CBlackwargreymonSkill3& Prototype)
	: CSkillObject{ Prototype }
{
}

HRESULT CBlackwargreymonSkill3::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBlackwargreymonSkill3::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC	Desc{};
	Desc.fRotationPerSec = XMConvertToRadians(180.0f);
	Desc.fSpeedPerSec = 10.f;

	m_fSpeed = 30.f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;


	m_pInteraction_Manager = CInteraction_Manager::GetInstance();
	m_pInteraction_Manager->Set_Attack_Digimon(this);

	const POSITION* Pos = static_cast<const POSITION*>(pArg);

	m_iDamage = Pos->iDamage;
	m_vPosition = Pos->m_vPosition;

	m_vTarget_pos = Pos->m_vTargetPosition;

	m_vPosition.m128_f32[1] += 10.f;

	m_vPosition.m128_f32[1] += 10.f;
	if (Pos->Look == 0)
	{
		m_vPosition.m128_f32[2] -= 15.f;
	}
	else if (Pos->Look == 1)
	{
		m_vPosition.m128_f32[2] += 15.f;
	}

	m_vFirst = Pos->m_vPosition;
	m_vFirst.m128_f32[1] += 60.f;

	m_pTransformCom->Set_State(STATE::POSITION, m_vPosition);
	m_pTransformCom->Update_WoldMatrix();

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	if (FAILED(Ready_SkillObjects()))
		return E_FAIL;

	return S_OK;
}

void CBlackwargreymonSkill3::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CBlackwargreymonSkill3::Update(_float fTimeDelta)
{

	m_vCurrent_pos = m_pTransformCom->Get_State(STATE::POSITION);
	if (m_bFirst == false)
	{
		_vector direction = XMVectorSubtract(m_vFirst, m_vCurrent_pos);
		float distance = XMVectorGetX(XMVector3Length(direction));

		if (distance < 0.001f) // 거의 도착하면
			return;

		_vector dirNormalized = XMVector3Normalize(direction);
		_vector move = XMVectorScale(dirNormalized, m_fSpeed * fTimeDelta);

		if (XMVectorGetX(XMVector3Length(move)) > distance)
		{
			m_vCurrent_pos = m_vFirst;
			m_bFirst = true;
		}
		else
		{
			m_vCurrent_pos = XMVectorAdd(m_vCurrent_pos, move);
			m_pTransformCom->Set_State((STATE::POSITION), m_vCurrent_pos);
		}
	}

	else if (m_bFirst && !m_bMove)
		m_pTransformCom->SizeUp(0.3f, 0.3f, 0.3f);


	if (m_bFirst && m_bMove)
		m_pTransformCom->Target_Pos_Move(m_vTarget_pos, m_fSpeed * fTimeDelta);
	

	m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));

	__super::Update(fTimeDelta);
}

void CBlackwargreymonSkill3::Late_Update(_float fTimeDelta)
{
	//m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
	__super::Late_Update(fTimeDelta);
}

HRESULT CBlackwargreymonSkill3::Render()
{
#ifdef _DEBUG
	m_pColliderCom->Render();
#endif

	return S_OK;
}

void CBlackwargreymonSkill3::Set_Pos(_float fX, _float fY)
{
	m_fX = fX;
	m_fY = fY;
}

_int CBlackwargreymonSkill3::Get_Damage()
{
	return m_iDamage;
}

HRESULT CBlackwargreymonSkill3::Ready_PartObjects()
{
	/* Com_Sphere*/
	CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};
	SphereDesc.fRadius = 0.f;
	SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius, 0.f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider_Sphere"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
		return E_FAIL;

	m_pInteraction_Manager->Set_Skill_Collider(m_pColliderCom);

	m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));

	return S_OK;
}

HRESULT CBlackwargreymonSkill3::Ready_SkillObjects()
{
	CBlackwargreymonSkill3_Part1::BODY_PLAYER_DESC Skill1{};

	Skill1.pParentTransform = m_pTransformCom;

	/* Part_Skill1 */
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_BlackwargreymonSkill3_Part1"),
		TEXT("Part_Skill1"), &Skill1)))
		return E_FAIL;

	return S_OK;
}

CBlackwargreymonSkill3* CBlackwargreymonSkill3::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBlackwargreymonSkill3* pInstance = new CBlackwargreymonSkill3(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBlackwargreymonSkill3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBlackwargreymonSkill3::Clone(void* pArg)
{
	CBlackwargreymonSkill3* pInstance = new CBlackwargreymonSkill3(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBlackwargreymonSkill3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBlackwargreymonSkill3::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);

}
