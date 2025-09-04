#include "UHD_Slot.h"

#include "GameInstance.h"

CUHD_Slot::CUHD_Slot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUHD_Slot::CUHD_Slot(const CUHD_Slot& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CUHD_Slot::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUHD_Slot::Initialize(void* pArg)
{
	CUIObject::UIOBJECT_DESC	Desc{};

	Desc.fX = 70.f;
	Desc.fY = 60.f;
	Desc.fSizeX = 102.f;
	Desc.fSizeY = 90.f;

	m_pRect = { long(Desc.fX - Desc.fSizeX * 0.5f), long(Desc.fY - Desc.fSizeY * 0.5f), long(Desc.fX + Desc.fSizeX * 0.5f), long(Desc.fY + Desc.fSizeY * 0.5f) };

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CUHD_Slot::Priority_Update(_float fTimeDelta)
{
}

void CUHD_Slot::Update(_float fTimeDelta)
{
}

void CUHD_Slot::Late_Update(_float fTimeDelta)
{
	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_fX - m_fWinSizeX * 0.5f, -m_fY + m_fWinSizeY * 0.5f, 0.f, 1.f));
}

HRESULT CUHD_Slot::Render()
{

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	__super::Begin();
	__super::Blend_Begin();

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	__super::Blend_End();
	__super::End();


	return S_OK;
}

void CUHD_Slot::Set_Move(_float fX, _float fY)
{
	m_fX = fX;
	m_fY = fY;

	m_pRect = { long(m_fX - m_fSizeX * 0.5f),long(m_fY - m_fSizeY * 0.5f),long(m_fX + m_fSizeX * 0.5f),long(m_fY + m_fSizeY * 0.5f) };
}

void CUHD_Slot::OnClick()
{
}

void CUHD_Slot::Set_Digimon_ID(_uint ID)
{
	m_iDigimon_ID = ID;
}

HRESULT CUHD_Slot::Ready_Components()
{
	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Mask_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_HUD_Mask"),
		TEXT("Com_Mask_Texture"), reinterpret_cast<CComponent**>(&m_pMaskTextureCom))))
		return E_FAIL;

	/* Com_Frame_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_HUD_Frame"),
		TEXT("Com_Frame_Texture"), reinterpret_cast<CComponent**>(&m_pFrameTextureCom))))
		return E_FAIL;

	/* Com_SP_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_HUD_SP"),
		TEXT("Com_SP_Texture"), reinterpret_cast<CComponent**>(&m_pSPTextureCom))))
		return E_FAIL;

	/* Com_HP_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_HUD_HP"),
		TEXT("Com_HP_Texture"), reinterpret_cast<CComponent**>(&m_pHPTextureCom))))
		return E_FAIL;

	/* Com_EVP_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_HUD_EVP"),
		TEXT("Com_EVP_Texture"), reinterpret_cast<CComponent**>(&m_pEVPTextureCom))))
		return E_FAIL;

	/* Com_Digimon_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Digimon_HUD"),
		TEXT("Com_Digimon_Texture"), reinterpret_cast<CComponent**>(&m_pDigimonTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_UHD"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

}

HRESULT CUHD_Slot::Bind_ShaderResources()
{
	/*m_pShaderCom->Bind_Matrix("g_WorldMatrix", );*/
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pMaskTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture1", 0)))
		return E_FAIL;
	if (FAILED(m_pFrameTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture2", 0)))
		return E_FAIL;
	if (FAILED(m_pSPTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture3", 0)))
		return E_FAIL;
	if (FAILED(m_pHPTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture4", 0)))
		return E_FAIL;
	if (FAILED(m_pEVPTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture5", 0)))
		return E_FAIL;
	if (FAILED(m_pDigimonTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture6", m_iDigimon_ID)))
		return E_FAIL;

	return S_OK;
}

CUHD_Slot* CUHD_Slot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUHD_Slot* pInstance = new CUHD_Slot(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUHD_Slot ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUHD_Slot::Clone(void* pArg)
{
	CUHD_Slot* pInstance = new CUHD_Slot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CUHD_Slot ");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CUHD_Slot::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pMaskTextureCom);
	Safe_Release(m_pFrameTextureCom);
	Safe_Release(m_pSPTextureCom);
	Safe_Release(m_pHPTextureCom);
	Safe_Release(m_pEVPTextureCom);
	Safe_Release(m_pDigimonTextureCom);
	Safe_Release(m_pShaderCom);
}
