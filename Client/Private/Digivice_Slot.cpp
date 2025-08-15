#include "Digivice_Slot.h"

#include "GameInstance.h"

CDigivice_Slot::CDigivice_Slot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUIObject{ pDevice, pContext }
{
}

CDigivice_Slot::CDigivice_Slot(const CDigivice_Slot& Prototype)
    : CUIObject{ Prototype }
{
}

HRESULT CDigivice_Slot::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CDigivice_Slot::Initialize(void* pArg)
{
	CUIObject::UIOBJECT_DESC	Desc{};

	Desc.fX = 100.f;
	Desc.fY = 100.f;
	Desc.fSizeX = 191.f;
	Desc.fSizeY = 39.f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;


	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CDigivice_Slot::Priority_Update(_float fTimeDelta)
{
}

void CDigivice_Slot::Update(_float fTimeDelta)
{
}

void CDigivice_Slot::Late_Update(_float fTimeDelta)
{
	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_fX - m_fSizeX * 0.5f, -m_fY + m_fSizeY * 0.5f, 0.f, 1.f) + XMLoadFloat4(&m_fParent_WorldPos));
}

HRESULT CDigivice_Slot::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	//__super::Begin();

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	//__super::End();

	return S_OK;
}

void CDigivice_Slot::Set_Parent_WorldPos(_vector fParent_World)
{
	XMStoreFloat4(&m_fParent_WorldPos, fParent_World);
}

void CDigivice_Slot::Set_Move(_float fX, _float fY)
{
	m_fX = fX;
	m_fY = fY;

	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_fX - m_fSizeX * 0.5f, -m_fY + m_fSizeY * 0.5f, 0.f, 1.f) + XMLoadFloat4(&m_fParent_WorldPos));

}


HRESULT CDigivice_Slot::Ready_Components()
{
	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Digivice_Slot"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CDigivice_Slot::Bind_ShaderResources()
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


CDigivice_Slot* CDigivice_Slot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDigivice_Slot* pInstance = new CDigivice_Slot(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDigivice_Slot ");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CDigivice_Slot::Clone(void* pArg)
{
	CDigivice_Slot* pInstance = new CDigivice_Slot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created :CDigivice_Slot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDigivice_Slot::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
