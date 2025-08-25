#include "DigiDex_Slot.h"

#include "GameInstance.h"

CDigiDex_Slot::CDigiDex_Slot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CDigiDex_Slot::CDigiDex_Slot(const CDigiDex_Slot& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CDigiDex_Slot::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDigiDex_Slot::Initialize(void* pArg)
{
	CUIObject::UIOBJECT_DESC	Desc{};

	Desc.fX = 100;
	Desc.fY = 100;
	Desc.fSizeX = 120.f;
	Desc.fSizeY = 120.f;

	m_pRect = { long(Desc.fX - Desc.fSizeX * 0.5f), long(Desc.fY - Desc.fSizeY * 0.5f), long(Desc.fX + Desc.fSizeX * 0.5f), long(Desc.fY + Desc.fSizeY * 0.5f) };

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;


	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CDigiDex_Slot::Priority_Update(_float fTimeDelta)
{
}

void CDigiDex_Slot::Update(_float fTimeDelta)
{
}

void CDigiDex_Slot::Late_Update(_float fTimeDelta)
{
	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_fX - m_fWinSizeX * 0.5f, -m_fY + m_fWinSizeY * 0.5f, 0.f, 1.f) + XMLoadFloat4(&m_fParent_WorldPos));
}

HRESULT CDigiDex_Slot::Render()
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

void CDigiDex_Slot::Set_Parent_WorldPos(_vector fParent_World)
{
	XMStoreFloat4(&m_fParent_WorldPos, fParent_World);
}

void CDigiDex_Slot::Set_Move(_float fX, _float fY)
{
	m_fX = fX;
	m_fY = fY;

	m_pRect = { long(m_fX - m_fSizeX * 0.5f),long(m_fY - m_fSizeY * 0.5f),long(m_fX + m_fSizeX * 0.5f),long(m_fY + m_fSizeY * 0.5f) };

}


void CDigiDex_Slot::OnClick()
{
}

HRESULT CDigiDex_Slot::Ready_Components()
{
	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Inventory_Slot"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_Hover"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CDigiDex_Slot::Bind_ShaderResources()
{/*m_pShaderCom->Bind_Matrix("g_WorldMatrix", );*/
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Int("g_Hover", m_bHover)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture1", 0)))
		return E_FAIL;

	return S_OK;
}

CDigiDex_Slot* CDigiDex_Slot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDigiDex_Slot* pInstance = new CDigiDex_Slot(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDigiDex_Slot ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDigiDex_Slot::Clone(void* pArg)
{
	CDigiDex_Slot* pInstance = new CDigiDex_Slot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CDigiDex_Slot ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDigiDex_Slot::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
