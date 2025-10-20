#include "BlackwargreymonSkill3.h"
#include "GameInstance.h"

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
	Desc.fSpeedPerSec = 50.f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet())
	m_pColliderCom->Set_Matrix();

	return S_OK;
}

void CBlackwargreymonSkill3::Priority_Update(_float fTimeDelta)
{

}

void CBlackwargreymonSkill3::Update(_float fTimeDelta)
{
	m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));
}

void CBlackwargreymonSkill3::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
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

HRESULT CBlackwargreymonSkill3::Ready_PartObjects()
{
	/* Com_Sphere*/
	CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};
	SphereDesc.fRadius = 50.f;
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
