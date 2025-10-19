#include "Digivice_Exp.h"
#include "GameInstance.h"

CDigivice_Exp::CDigivice_Exp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CDigivice_Exp::CDigivice_Exp(const CDigivice_Exp& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CDigivice_Exp::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDigivice_Exp::Initialize(void* pArg)
{
	CUIObject::UIOBJECT_DESC	Desc{};

	Desc.fX = 100.f;
	Desc.fY = 300.f;
	Desc.fSizeX = 260.f;
	Desc.fSizeY = 16.f;

	m_pRect = { long(Desc.fX - Desc.fSizeX * 0.5f), long(Desc.fY - Desc.fSizeY * 0.5f), long(Desc.fX + Desc.fSizeX * 0.5f), long(Desc.fY + Desc.fSizeY * 0.5f) };

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CDigivice_Exp::Priority_Update(_float fTimeDelta)
{
}

void CDigivice_Exp::Update(_float fTimeDelta)
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

	m_fExpRatio = m_iCurrentExp / static_cast<_float>(m_iMaxExp);
}

void CDigivice_Exp::Late_Update(_float fTimeDelta)
{
}


HRESULT CDigivice_Exp::Render()
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

void CDigivice_Exp::Set_MaxExp(_int MaxExp)
{
	m_iMaxExp = MaxExp;
}

void CDigivice_Exp::Set_Move(_float fX, _float fY)
{
	m_fX = fX;
	m_fY = fY;
}

void CDigivice_Exp::Set_Exp(_int m_iExp)
{
	m_iCurrentExp = m_iExp;
}

HRESULT CDigivice_Exp::Ready_Components()
{
	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Digivice_Exp*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Digivice_Status_EXP"),
		TEXT("Com_Digivice_Exp"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_Digivice_Epx_bg*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Digivice_Status_BG"),
		TEXT("Com_Digivice_Epx_bg"), reinterpret_cast<CComponent**>(&m_pBgTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_Digivice_State"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}
HRESULT CDigivice_Exp::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_State("Current", m_fExpRatio)))
		return E_FAIL;
	if (FAILED(m_pBgTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture1", 0)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture2", 0)))
		return E_FAIL;
	return S_OK;
}

CDigivice_Exp* CDigivice_Exp::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDigivice_Exp* pInstance = new CDigivice_Exp(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDigivice_Exp ");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CDigivice_Exp::Clone(void* pArg)
{
	CDigivice_Exp* pInstance = new CDigivice_Exp(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created :CDigivice_Exp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDigivice_Exp::Free()
{
	__super::Free();

	Safe_Release(m_pBgTextureCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
}
