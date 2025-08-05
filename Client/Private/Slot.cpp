#include "Slot.h"

#include "GameInstance.h"

CSlot::CSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CSlot::CSlot(const CSlot& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CSlot::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSlot::Initialize(void* pArg)
{
	CUIObject::UIOBJECT_DESC	Desc{};

	Desc.fX = 100.f;
	Desc.fY = 100.f;
	Desc.fSizeX = 30.f;
	Desc.fSizeY = 30.f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;


	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CSlot::Priority_Update(_float fTimeDelta)
{
}

void CSlot::Update(_float fTimeDelta)
{
}

void CSlot::Late_Update(_float fTimeDelta)
{
	m_pTransformCom->Set_State();

	m_pGameInstance->Add_RenderGroup(RENDER::BLENDUI, this);
}

HRESULT CSlot::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	__super::Begin();

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	__super::End();

	return S_OK;
}


HRESULT CSlot::Ready_Components()
{
	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Slot"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CSlot::Bind_ShaderResources()
{
	/*m_pShaderCom->Bind_Matrix("g_WorldMatrix", );*/
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	return S_OK;
}

CSlot* CSlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSlot* pInstance = new CSlot(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSlot ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSlot::Clone(void* pArg)
{
	CSlot* pInstance = new CSlot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created :CSlot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSlot::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
