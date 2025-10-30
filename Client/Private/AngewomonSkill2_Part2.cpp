#include "AngewomonSkill2_Part2.h"
#include "GameInstance.h"

CAngewomonSkill2_Part2::CAngewomonSkill2_Part2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject{ pDevice, pContext }
{
}

CAngewomonSkill2_Part2::CAngewomonSkill2_Part2(const CAngewomonSkill2_Part2& Prototype)
	: CPartObject{ Prototype }
{
}

_float4x4* CAngewomonSkill2_Part2::Get_BoneMatrixPtr(const _char* pBoneName)
{
	return m_pModelCom->Get_BoneMatrixPtr(pBoneName);
}

HRESULT CAngewomonSkill2_Part2::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAngewomonSkill2_Part2::Initialize(void* pArg)
{
	BODY_PLAYER_DESC* pDesc = static_cast<BODY_PLAYER_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CAngewomonSkill2_Part2::Priority_Update(_float fTimeDelta)
{
}

void CAngewomonSkill2_Part2::Update(_float fTimeDelta)
{
	if (m_bHit)
		m_fTime += fTimeDelta;

	XMStoreFloat4x4(&m_CombinedWorldMatrix,
		XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()) * XMLoadFloat4x4(m_pParentTransformCom->Get_WorldMatrixPtr()));
}

void CAngewomonSkill2_Part2::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::EFFECT, this);
	m_pGameInstance->Add_RenderGroup(RENDER::BLUR, this);
}

HRESULT CAngewomonSkill2_Part2::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(2)))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(0)))
		return E_FAIL;

	return S_OK;
}

HRESULT CAngewomonSkill2_Part2::Ready_Components()
{
	/* Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Angewomon_Skill2_2"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_AngewomonSkill2_Image"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_DissloveTexture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Noise"),
		TEXT("Com_DissloveTexture"), reinterpret_cast<CComponent**>(&m_pDIssolveTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxSkillAngewomon"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CAngewomonSkill2_Part2::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_State("Time", m_fTime)))
		return E_FAIL;
	if (FAILED(m_pDIssolveTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Dissolve", 10)))
		return E_FAIL;

	return S_OK;
}

CAngewomonSkill2_Part2* CAngewomonSkill2_Part2::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CAngewomonSkill2_Part2* pInstance = new CAngewomonSkill2_Part2(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CAngewomonSkill2_Part2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CAngewomonSkill2_Part2::Clone(void* pArg)
{
	CAngewomonSkill2_Part2* pInstance = new CAngewomonSkill2_Part2(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CAngewomonSkill2_Part2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAngewomonSkill2_Part2::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pDIssolveTextureCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
