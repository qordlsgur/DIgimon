#include "ItemType_Button.h"

#include "GameInstance.h"

CItemType_Button::CItemType_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CButton{ pDevice, pContext }
{
}

CItemType_Button::CItemType_Button(const CItemType_Button& Prototype)
	: CButton{ Prototype }
{
}

HRESULT CItemType_Button::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CItemType_Button::Initialize(void* pArg)
{

	CButton::BUTTON_DESC	Desc{};

	Desc.fX = 100.f;
	Desc.fY = 100.f;
	Desc.fSizeX = 80.f;
	Desc.fSizeY = 40.f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;


	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_bHover = true;

	return S_OK;
}

void CItemType_Button::Priority_Update(_float fTimeDelta)
{
}

void CItemType_Button::Update(_float fTimeDelta)
{
}

void CItemType_Button::Late_Update(_float fTimeDelta)
{
	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_fX - m_fSizeX * 0.5f, -m_fY + m_fSizeY * 0.5f, 0.f, 1.f) + XMLoadFloat4(&m_fParent_WorldPos));
}


HRESULT CItemType_Button::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(1)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	__super::Begin();

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	__super::End();

	return S_OK;
}

void CItemType_Button::Set_Parent_WorldPos(_vector fParent_World)
{
	XMStoreFloat4(&m_fParent_WorldPos, fParent_World);
}
void CItemType_Button::Set_Move(_float fX, _float fY)
{
	m_fX = fX;
	m_fY = fY;

	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_fX - m_fSizeX * 0.5f, -m_fY + m_fSizeY * 0.5f, 0.f, 1.f) + XMLoadFloat4(&m_fParent_WorldPos));
}

HRESULT CItemType_Button::Ready_Components()
{
	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_ItemType_Button"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxNorTexTow"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CItemType_Button::Bind_ShaderResources()
{
	/*m_pShaderCom->Bind_Matrix("g_WorldMatrix", );*/
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture1", 0)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture2", 1)))
		return E_FAIL;


	return S_OK;
}

CItemType_Button* CItemType_Button::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CItemType_Button* pInstance = new CItemType_Button(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CItemType_Button ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CItemType_Button::Clone(void* pArg)
{
	CItemType_Button* pInstance = new CItemType_Button(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created :CItemType_Button");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItemType_Button::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}