#include "Digivice_Info.h"
#include "GameInstance.h"

CDigivice_Info::CDigivice_Info(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CDigivice_Info::CDigivice_Info(const CDigivice_Info& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CDigivice_Info::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDigivice_Info::Initialize(void* pArg)
{
	CUIObject::UIOBJECT_DESC	Desc{};

	Desc.fX = 100.f;
	Desc.fY = 100.f;
	Desc.fSizeX = 290.f;
	Desc.fSizeY = 500.f;

	m_pRect = { long(Desc.fX - Desc.fSizeX * 0.5f), long(Desc.fY - Desc.fSizeY * 0.5f), long(Desc.fX + Desc.fSizeX * 0.5f), long(Desc.fY + Desc.fSizeY * 0.5f) };

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}


void CDigivice_Info::Priority_Update(_float fTimeDelta)
{
}

void CDigivice_Info::Update(_float fTimeDelta)
{
}

void CDigivice_Info::Late_Update(_float fTimeDelta)
{
	_vector vWorldPos = XMVectorSet(m_fX + m_fParent_WorldPos.x, -(m_fY)+m_fParent_WorldPos.y, 0.f, 1.f);

	m_pTransformCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(STATE::POSITION, vWorldPos);

	_float4 Pos;
	XMStoreFloat4(&Pos, vWorldPos);

	m_pRect = {
		long(Pos.x - m_fSizeX * 0.5f),
		long(-Pos.y - m_fSizeY * 0.5f),
		long(Pos.x + m_fSizeX * 0.5f),
		long(-Pos.y + m_fSizeY * 0.5f)
	};
}

HRESULT CDigivice_Info::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(1)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

void CDigivice_Info::Set_Parent_WorldPos(_vector fParent_World)
{
	XMStoreFloat4(&m_fParent_WorldPos, fParent_World);
}

void CDigivice_Info::Set_Move(_float fX, _float fY)
{
	m_fX = fX;
	m_fY = fY;

	m_pRect = { long(m_fX - m_fSizeX * 0.5f), long(m_fY - m_fSizeY * 0.5f), long(m_fX + m_fSizeX * 0.5f), long(m_fY + m_fSizeY * 0.5f) };
}

HRESULT CDigivice_Info::Ready_Components()
{
	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Digivice_Info"),
		TEXT("Com_DTexture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_Inventory"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CDigivice_Info::Bind_ShaderResources()
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
	return S_OK;
}

CDigivice_Info* CDigivice_Info::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDigivice_Info* pInstance = new CDigivice_Info(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDigivice_Info ");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CDigivice_Info::Clone(void* pArg)
{
	CDigivice_Info* pInstance = new CDigivice_Info(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created :CDigivice_Info");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDigivice_Info::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
}
