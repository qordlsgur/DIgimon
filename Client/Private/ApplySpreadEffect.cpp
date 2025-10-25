#include "ApplySpreadEffect.h"
#include "GameInstance.h"

CApplySpreadEffect::CApplySpreadEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEffect{ pDevice, pContext }
{
}

CApplySpreadEffect::CApplySpreadEffect(const CApplySpreadEffect& Prototype)
	: CEffect{ Prototype }
{
}

HRESULT CApplySpreadEffect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CApplySpreadEffect::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Scale(100.f, 100.f, 100.f);

	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(10.f, 3.f, 500.f, 1.f));

	return S_OK;
}

void CApplySpreadEffect::Priority_Update(_float fTimeDelta)
{
}

void CApplySpreadEffect::Update(_float fTimeDelta)
{
}

void CApplySpreadEffect::Late_Update(_float fTimeDelta)
{
	Compute_Depth();

	m_pGameInstance->Add_RenderGroup(RENDER::BATTLEUI, this);
}

HRESULT CApplySpreadEffect::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CApplySpreadEffect::Ready_Components()
{
	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Battle_HpBG*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Fire4x4"),
		TEXT("Com_Battle_HpBG"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CApplySpreadEffect::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RenderTarget(TEXT("Target_Depth"), m_pShaderCom, "g_DepthTexture")))
		return E_FAIL;
	return S_OK;
}

CApplySpreadEffect* CApplySpreadEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CApplySpreadEffect* pInstance = new CApplySpreadEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CApplySpreadEffect ");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CApplySpreadEffect::Clone(void* pArg)
{
	CApplySpreadEffect* pInstance = new CApplySpreadEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created :CBattle_Turn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CApplySpreadEffect::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
}

