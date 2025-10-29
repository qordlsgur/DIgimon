#include "MetalgreymonSkill3_Part3.h"
#include "GameInstance.h"

CMetalgreymonSkill3_Part3::CMetalgreymonSkill3_Part3(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject{ pDevice, pContext }
{
}

CMetalgreymonSkill3_Part3::CMetalgreymonSkill3_Part3(const CMetalgreymonSkill3_Part3& Prototype)
	: CPartObject{ Prototype }
{
}

_float4x4* CMetalgreymonSkill3_Part3::Get_BoneMatrixPtr(const _char* pBoneName)
{
	return nullptr;
}

HRESULT CMetalgreymonSkill3_Part3::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMetalgreymonSkill3_Part3::Initialize(void* pArg)
{
	BODY_PLAYER_DESC* pDesc = static_cast<BODY_PLAYER_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Scale(10.0f, 10.f, 10.f);
	m_fTime = 0.f;

	return S_OK;
}

void CMetalgreymonSkill3_Part3::Priority_Update(_float fTimeDelta)
{
}

void CMetalgreymonSkill3_Part3::Update(_float fTimeDelta)
{
	m_fTime += fTimeDelta * 5.f;

	XMStoreFloat4x4(&m_CombinedWorldMatrix,
		XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()) * XMLoadFloat4x4(m_pParentTransformCom->Get_WorldMatrixPtr()));


}

void CMetalgreymonSkill3_Part3::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
}

HRESULT CMetalgreymonSkill3_Part3::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(4)))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(0)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMetalgreymonSkill3_Part3::Ready_Components()
{
	/* Com_Mode */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Plane"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_MetalgreymonSkill3_Image"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxSkillMetalgarumon"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMetalgreymonSkill3_Part3::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_State("Time", m_fTime)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_State("Count", 2)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_State("Frame", 0.2f)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 2)))
		return E_FAIL;

	return S_OK;
}

CMetalgreymonSkill3_Part3* CMetalgreymonSkill3_Part3::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMetalgreymonSkill3_Part3* pInstance = new CMetalgreymonSkill3_Part3(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMetalgreymonSkill3_Part3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMetalgreymonSkill3_Part3::Clone(void* pArg)
{
	CMetalgreymonSkill3_Part3* pInstance = new CMetalgreymonSkill3_Part3(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMetalgreymonSkill3_Part3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMetalgreymonSkill3_Part3::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
