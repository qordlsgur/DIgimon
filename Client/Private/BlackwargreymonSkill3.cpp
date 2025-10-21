#include "BlackwargreymonSkill3.h"
#include "GameInstance.h"
#include "Interaction_Manager.h"

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

	m_fSpeed = 40.f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	m_pInteraction_Manager = CInteraction_Manager::GetInstance();
	m_pInteraction_Manager->Set_Attack_Digimon(this);
	m_pInteraction_Manager->Set_Skill_Collider(m_pColliderCom);

	const POSITION* Pos = static_cast<const POSITION*>(pArg);

	m_iDamage = Pos->iDamage;
	m_vPosition = Pos->m_vPosition;

	m_pTarget_pos = Pos->m_vTargetPosition;

	m_vPosition.m128_f32[1] += 10.f;
	m_vPosition.m128_f32[2] += 20.f;

	m_vFirst = Pos->m_vPosition;
	m_vFirst.m128_f32[1] += 50.f;

	m_pTransformCom->Set_State(STATE::POSITION, m_vPosition);

	return S_OK;
}

void CBlackwargreymonSkill3::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CBlackwargreymonSkill3::Update(_float fTimeDelta)
{
	if (m_bHit)
	{

	}
	Pos = m_pTransformCom->Get_State(STATE::POSITION);
	if (m_bFirst == false)
	{
		XMVECTOR direction = XMVectorSubtract(m_vFirst, Pos);  // 
		float distance = XMVectorGetX(XMVector3Length(direction)); 

		if (distance < 0.001f) // 거의 도착하면
			return;

		XMVECTOR dirNormalized = XMVector3Normalize(direction); 
		XMVECTOR move = XMVectorScale(dirNormalized, m_fSpeed * fTimeDelta);

		if (XMVectorGetX(XMVector3Length(move)) > distance) 
		{
			Pos = m_vFirst;
			m_bFirst = true;
		}
		else
			Pos = XMVectorAdd(Pos, move);
	}

	else if (m_bFirst && !m_bSize)
	{
		m_pTransformCom->SizeUp(0.023f, 0.023f, 0.023f);

		if (m_pTransformCom->Get_Scale().x >= 3.f)
			m_bSize = true;
	}

	if (m_bFirst && m_bSize && !m_bEnd)
	{
		XMVECTOR direction = XMVectorSubtract(m_pTarget_pos, Pos);
		float distance = XMVectorGetX(XMVector3Length(direction));

		if (distance < 0.001f)
			return;

		XMVECTOR dirNormalized = XMVector3Normalize(direction);
		XMVECTOR move = XMVectorScale(dirNormalized, m_fSpeed * fTimeDelta * 3.5f);

		if (XMVectorGetX(XMVector3Length(move)) > distance)
		{
			Pos = m_pTarget_pos;
			m_bEnd = true;
		}
		else
			Pos = XMVectorAdd(Pos, move); // 이동
	}

	m_pTransformCom->Set_State((STATE::POSITION), Pos);

	m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));

	__super::Update(fTimeDelta);
}

void CBlackwargreymonSkill3::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
	__super::Priority_Update(fTimeDelta);
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
	SphereDesc.fRadius = 10.f;
	SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius, 0.f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider_Sphere"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
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
