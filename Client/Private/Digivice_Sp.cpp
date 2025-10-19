#include "Digivice_Sp.h"
#include "GameInstance.h"

CDigivice_Sp::CDigivice_Sp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CDigivice_Sp::CDigivice_Sp(const CDigivice_Sp& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CDigivice_Sp::Initialize_Prototype()
{
	return S_OK;
}


HRESULT CDigivice_Sp::Initialize(void* pArg)
{
	CUIObject::UIOBJECT_DESC	Desc{};

	Desc.fX = 100.f;
	Desc.fY = 200.f;
	Desc.fSizeX = 260.f;
	Desc.fSizeY = 16.f;

	m_pRect = { long(Desc.fX - Desc.fSizeX * 0.5f), long(Desc.fY - Desc.fSizeY * 0.5f), long(Desc.fX + Desc.fSizeX * 0.5f), long(Desc.fY + Desc.fSizeY * 0.5f) };

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CDigivice_Sp::Priority_Update(_float fTimeDelta)
{
}

void CDigivice_Sp::Update(_float fTimeDelta)
{
	_vector vWorldPos = XMVectorSet(m_fX - m_fWinSizeX * 0.5f, -m_fY + m_fWinSizeY * 0.5f, 0.f, 1.f);

	m_pTransformCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(STATE::POSITION, vWorldPos);

	_float4 Pos;
	XMStoreFloat4(&Pos, vWorldPos);

	m_pRect = {
		long(m_fX - m_fSizeX * 0.5f),
		long(m_fY - m_fSizeY * 0.5f),
		long(m_fX + m_fSizeX * 0.5f),
		long(m_fY + m_fSizeY * 0.5f)
	};


	m_fSpRatio = m_iCurrentSp / static_cast<_float>(m_iMaxSp);


}

void CDigivice_Sp::Late_Update(_float fTimeDelta)
{
}

HRESULT CDigivice_Sp::Render()
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

void CDigivice_Sp::Set_MaxSp(_int MaxSp)
{
	m_iMaxSp = MaxSp;
	m_iCurrentSp = MaxSp;
}

void CDigivice_Sp::Set_Move(_float fX, _float fY)
{
	m_fX = fX;
	m_fY = fY;
}

HRESULT CDigivice_Sp::Ready_Components()
{
	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Digivice_Sp*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Digivice_Status_SP"),
		TEXT("Com_Digivice_Sp"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_Digivice_Sp_bg*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Digivice_Status_BG"),
		TEXT("Com_Digivice_Epx_bg"), reinterpret_cast<CComponent**>(&m_pBgTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_Digivice_State"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CDigivice_Sp::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_State("Current", m_fSpRatio)))
		return E_FAIL;
	if (FAILED(m_pBgTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture1", 0)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture2", 0)))
		return E_FAIL;

	return S_OK;
}
CDigivice_Sp* CDigivice_Sp::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDigivice_Sp* pInstance = new CDigivice_Sp(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDigivice_Sp ");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CDigivice_Sp::Clone(void* pArg)
{
	CDigivice_Sp* pInstance = new CDigivice_Sp(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created :CDigivice_Sp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDigivice_Sp::Free()
{
	__super::Free();

	Safe_Release(m_pBgTextureCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
}
