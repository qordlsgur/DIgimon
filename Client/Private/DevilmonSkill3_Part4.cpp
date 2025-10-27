#include "DevilmonSkill3_Part4.h"
#include "GameInstance.h"
#include "Interaction_Manager.h"

CDevilmonSkill3_Part4::CDevilmonSkill3_Part4(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject{ pDevice, pContext }
{
}

CDevilmonSkill3_Part4::CDevilmonSkill3_Part4(const CDevilmonSkill3_Part4& Prototype)
	: CPartObject{ Prototype }
{
}

_float4x4* CDevilmonSkill3_Part4::Get_BoneMatrixPtr(const _char* pBoneName)
{
	return nullptr;
}

HRESULT CDevilmonSkill3_Part4::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDevilmonSkill3_Part4::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC	Desc{};
	Desc.fRotationPerSec = XMConvertToRadians(180.0f);
	Desc.fSpeedPerSec = 30.f;

	BODY_PLAYER_DESC* pDesc = static_cast<BODY_PLAYER_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pInteraction_Manager = CInteraction_Manager::GetInstance();
	m_pInteraction_Manager->Set_Attack_Skill(this);

	if (FAILED(Ready_Components()))
		return E_FAIL;


	m_vPosition = pDesc->vPosition;
	m_vTargetPosition = pDesc->vPosition2;
	m_pTransformCom->Set_State(STATE::POSITION, m_vPosition);

	m_fTime = 0.f;
	return S_OK;
}
void CDevilmonSkill3_Part4::Priority_Update(_float fTimeDelta)
{
}

void CDevilmonSkill3_Part4::Update(_float fTimeDelta)
{
	//m_fTime += fTimeDelta;

	//_vector Pos = m_pTransformCom->Get_State(STATE::POSITION);
	//m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(Pos.m128_f32[0], m_fTime * -2, Pos.m128_f32[2], 1.f));

	//XMStoreFloat4x4(&m_CombinedWorldMatrix,
	//	XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()) * XMLoadFloat4x4(m_pParentTransformCom->Get_WorldMatrixPtr()));


	m_vScale = XMVectorSet(m_pTransformCom->Get_Scale().x, m_pTransformCom->Get_Scale().y, m_pTransformCom->Get_Scale().z, 1.f);

	m_pTransformCom->Target_Pos_Move_Speed(m_vTargetPosition, 100.f,fTimeDelta);
	m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));

}

void CDevilmonSkill3_Part4::Late_Update(_float fTimeDelta)
{
	Compute_Depth();

	m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
}

HRESULT CDevilmonSkill3_Part4::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(1)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

#ifdef _DEBUG
	m_pColliderCom->Render();
#endif
	return S_OK;
}

HRESULT CDevilmonSkill3_Part4::Ready_Components()
{
	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	///* Com_Model */
	//if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Disc"),
	//	TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
	//	return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_DevilmonSkill3_Image"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxEffectDevilmon"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Sphere*/
	CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};
	SphereDesc.fRadius = 5.f;
	SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius, 0.f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider_Sphere"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
		return E_FAIL;

	m_pInteraction_Manager->Set_Skill_Collider(m_pColliderCom);
	m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));

	return S_OK;
}

HRESULT CDevilmonSkill3_Part4::Bind_ShaderResources()
{
	/*if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;*/
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Vector("g_Scale", &m_vScale)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_vCamPosition", m_pGameInstance->Get_Transform_Float4x4_Inverse(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RenderTarget(TEXT("Target_Depth"), m_pShaderCom, "g_DepthTexture")))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 1)))
		return E_FAIL;
	return S_OK;
}

CDevilmonSkill3_Part4* CDevilmonSkill3_Part4::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDevilmonSkill3_Part4* pInstance = new CDevilmonSkill3_Part4(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDevilmonSkill3_Part4");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDevilmonSkill3_Part4::Clone(void* pArg)
{
	CDevilmonSkill3_Part4* pInstance = new CDevilmonSkill3_Part4(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDevilmonSkill3_Part4");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDevilmonSkill3_Part4::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
