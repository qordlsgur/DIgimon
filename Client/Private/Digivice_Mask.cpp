#include "Digivice_Mask.h"

#include "GameInstance.h"

CDigivice_Mask::CDigivice_Mask(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CDigivice_Mask::CDigivice_Mask(const CDigivice_Mask& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CDigivice_Mask::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDigivice_Mask::Initialize(void* pArg)
{
	CUIObject::UIOBJECT_DESC	Desc{};

	Desc.fX = 100.f;
	Desc.fY = 100.f;
	Desc.fSizeX = 160.f;
	Desc.fSizeY = 90.f;

	m_pRect = { long(Desc.fX - Desc.fSizeX * 0.5f), long(Desc.fY - Desc.fSizeY * 0.5f), long(Desc.fX + Desc.fSizeX * 0.5f), long(Desc.fY + Desc.fSizeY * 0.5f) };

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;


	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_bHasDigimon = true;

	return S_OK;
}

void CDigivice_Mask::Priority_Update(_float fTimeDelta)
{
}

void CDigivice_Mask::Update(_float fTimeDelta)
{
	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_fX - m_fWinSizeX * 0.5f, -m_fY + m_fWinSizeY * 0.5f, 0.f, 1.f) + XMLoadFloat4(&m_fParent_WorldPos));
	m_pTransformCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
}

void CDigivice_Mask::Late_Update(_float fTimeDelta)
{
}

HRESULT CDigivice_Mask::Render()
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

void CDigivice_Mask::Set_Parent_WorldPos(_vector fParent_World)
{
	XMStoreFloat4(&m_fParent_WorldPos, fParent_World);
}

void CDigivice_Mask::Set_Move(_float fX, _float fY)
{
	m_fX = fX;
	m_fY = fY;

	m_pRect = { long(m_fX - m_fSizeX * 0.5f), long(m_fY - m_fSizeY * 0.5f), long(m_fX + m_fSizeX * 0.5f), long(m_fY + m_fSizeY * 0.5f) };

}

HRESULT CDigivice_Mask::Ready_Components()
{
	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Digivice_Mask_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Digivice_Mask"),
		TEXT("Com_Digivice_Mask_Texture"), reinterpret_cast<CComponent**>(&m_pMaskTextureCom))))
		return E_FAIL;

	/* Com_Digivice_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Digimon_Digivice"),
		TEXT("Com_Digivice_Texture"), reinterpret_cast<CComponent**>(&m_pDigimonTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_Digivice_Mask"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CDigivice_Mask::Bind_ShaderResources()
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

	if (m_bHasDigimon)
	{
		m_pShaderCom->Bind_Int("HasDigimon", 1);
		if (FAILED(m_pDigimonTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture2", 5)))
			return E_FAIL;
	}

	return S_OK;
}

CDigivice_Mask* CDigivice_Mask::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDigivice_Mask* pInstance = new CDigivice_Mask(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDigivice_Mask ");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CDigivice_Mask::Clone(void* pArg)
{
	CDigivice_Mask* pInstance = new CDigivice_Mask(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created :CDigivice_Mask");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDigivice_Mask::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pMaskTextureCom);
	Safe_Release(m_pDigimonTextureCom);
	Safe_Release(m_pShaderCom);
}
