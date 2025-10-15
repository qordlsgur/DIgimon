#include "Digivice_Hp.h"
#include "GameInstance.h"

CDigivice_Hp::CDigivice_Hp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CDigivice_Hp::CDigivice_Hp(const CDigivice_Hp& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CDigivice_Hp::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDigivice_Hp::Initialize(void* pArg)
{
	CUIObject::UIOBJECT_DESC	Desc{};

	Desc.fX = 100.f;
	Desc.fY = 100.f;
	Desc.fSizeX = 260.f;
	Desc.fSizeY = 16.f;

	m_pRect = { long(Desc.fX - Desc.fSizeX * 0.5f), long(Desc.fY - Desc.fSizeY * 0.5f), long(Desc.fX + Desc.fSizeX * 0.5f), long(Desc.fY + Desc.fSizeY * 0.5f) };

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CDigivice_Hp::Priority_Update(_float fTimeDelta)
{
}

void CDigivice_Hp::Update(_float fTimeDelta)
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

	if (m_iMaxHp != 0)
	{
		if (m_pGameInstance->Key_Down(DIK_I))
			m_iCurrentHp = 5000.f;

		if (m_pGameInstance->Key_Down(DIK_O))
			m_iCurrentHp += 500.f;
	}

	m_fHpRatio = m_iCurrentHp / m_iMaxHp;

}

void CDigivice_Hp::Late_Update(_float fTimeDelta)
{
}


HRESULT CDigivice_Hp::Render()
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

void CDigivice_Hp::Set_MaxHp(_float MaxHp)
{
	m_iMaxHp = MaxHp;
	m_iCurrentHp = MaxHp;
}

void CDigivice_Hp::Set_Move(_float fX, _float fY)
{
	m_fX = fX;
	m_fY = fY;
}

HRESULT CDigivice_Hp::Ready_Components()
{
	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Digivice_Hp*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Digivice_Status_HP"),
		TEXT("Com_Digivice_Hp"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_Digivice_Hp_bg*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Digivice_Status_BG"),
		TEXT("Com_Digivice_Hp_bg"), reinterpret_cast<CComponent**>(&m_pBgTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_Digivice_State"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CDigivice_Hp::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_State("Current", m_fHpRatio)))
		return E_FAIL;
	if (FAILED(m_pBgTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture1", 0)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture2", 0)))
		return E_FAIL;

	return S_OK;
}

CDigivice_Hp* CDigivice_Hp::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDigivice_Hp* pInstance = new CDigivice_Hp(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBattle_Turn ");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CDigivice_Hp::Clone(void* pArg)
{
	CDigivice_Hp* pInstance = new CDigivice_Hp(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created :CDigivice_Hp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDigivice_Hp::Free()
{
	__super::Free();

	Safe_Release(m_pBgTextureCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
}
