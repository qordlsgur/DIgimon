#include "WargreymonSkill2.h"
#include "GameInstance.h"
#include "Interaction_Manager.h"

CWargreymonSkill2::CWargreymonSkill2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CSkillObject{ pDevice, pContext }
{
}

CWargreymonSkill2::CWargreymonSkill2(const CWargreymonSkill2& Prototype)
    : CSkillObject{ Prototype }
{
}

HRESULT CWargreymonSkill2::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CWargreymonSkill2::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC	Desc{};
	Desc.fRotationPerSec = XMConvertToRadians(180.0f);
	Desc.fSpeedPerSec = 10.f;

	m_fSpeed = 40.f;

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
		m_vPosition.m128_f32[2] -= 20.f;
	}
	else if (Pos->Look == 1)
	{
		m_vPosition.m128_f32[2] += 20.f;
	}

	m_vFirst = Pos->m_vPosition;
	m_vFirst.m128_f32[1] += 50.f;

	m_pTransformCom->Set_State(STATE::POSITION, m_vPosition);


	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	return S_OK;
}
void CWargreymonSkill2::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CWargreymonSkill2::Update(_float fTimeDelta)
{
	if (m_bHit)
	{

	}
	m_vCurrent_pos = m_pTransformCom->Get_State(STATE::POSITION);
	if (m_bFirst == false)
	{
		_vector direction = XMVectorSubtract(m_vFirst, m_vCurrent_pos);  // 
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
			m_vCurrent_pos = XMVectorAdd(m_vCurrent_pos, move);
	}

	else if (m_bFirst && !m_bSize)
	{
		m_pTransformCom->SizeUp(0.023f, 0.023f, 0.023f);

		if (m_pTransformCom->Get_Scale().x >= 3.f)
			m_bSize = true;
	}

	if (m_bFirst && m_bSize && !m_bEnd)
	{
		_vector direction = XMVectorSubtract(m_vTarget_pos, m_vCurrent_pos);
		_float distance = XMVectorGetX(XMVector3Length(direction));

		if (distance < 0.001f)
			return;

		_vector dirNormalized = XMVector3Normalize(direction);
		_vector move = XMVectorScale(dirNormalized, m_fSpeed * fTimeDelta * 3.5f);

		if (XMVectorGetX(XMVector3Length(move)) > distance)
		{
			m_vCurrent_pos = m_vTarget_pos;
			m_bEnd = true;
		}
		else
			m_vCurrent_pos = XMVectorAdd(m_vCurrent_pos, move); // 이동
	}

	m_pTransformCom->Set_State((STATE::POSITION), m_vCurrent_pos);

	m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));

	__super::Update(fTimeDelta);
}

void CWargreymonSkill2::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
	__super::Priority_Update(fTimeDelta);
}

HRESULT CWargreymonSkill2::Render()
{
#ifdef _DEBUG
	m_pColliderCom->Render();
#endif

	return S_OK;
}

_int CWargreymonSkill2::Get_Damage()
{
	return 
		m_iDamage;
}


HRESULT CWargreymonSkill2::Ready_PartObjects()
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

CWargreymonSkill2* CWargreymonSkill2::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWargreymonSkill2* pInstance = new CWargreymonSkill2(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWargreymonSkill2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWargreymonSkill2::Clone(void* pArg)
{
	CWargreymonSkill2* pInstance = new CWargreymonSkill2(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWargreymonSkill2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWargreymonSkill2::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);

}