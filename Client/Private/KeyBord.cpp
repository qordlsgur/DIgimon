#include "KeyBord.h"
#include "GameInstance.h"

CKeyBord::CKeyBord(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CKeyBord::CKeyBord(const CKeyBord& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CKeyBord::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CKeyBord::Initialize(void* pArg)
{
	CUIObject::UIOBJECT_DESC	Desc{};

	Desc.fX = 500.f;
	Desc.fY = 500.f;
	Desc.fSizeX = 32.f;
	Desc.fSizeY = 32.f;

	m_pRect = { long(Desc.fX - Desc.fSizeX * 0.5f), long(Desc.fY - Desc.fSizeY * 0.5f), long(Desc.fX + Desc.fSizeX * 0.5f), long(Desc.fY + Desc.fSizeY * 0.5f) };

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CKeyBord::Priority_Update(_float fTimeDelta)
{
}

void CKeyBord::Update(_float fTimeDelta)
{
	_vector vWorldPos = XMVectorSet(m_fX - m_fWinSizeX * 0.5f, -m_fY + m_fWinSizeY * 0.5f, 0.f, 1.f);

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
	OnHover();
}

void CKeyBord::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::BATTLEUI, this);
}

HRESULT CKeyBord::Render()
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

void CKeyBord::Set_KeyBord(_int Key)
{
	m_iKey = Key;
}

void CKeyBord::Set_Move(_float fX, _float fY)
{
	m_fX = fX;
	m_fY = fY;
}

HRESULT CKeyBord::Ready_Components()
{
	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Skill_KeyBord"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_KeyBord_Select*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Select_Key"),
		TEXT("Com_KeyBord_Select"), reinterpret_cast<CComponent**>(&m_pSelectTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_Skill_Hover"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CKeyBord::Bind_ShaderResources()
{
	/*m_pShaderCom->Bind_Matrix("g_WorldMatrix", );*/
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Int("HasDigimon", m_bHover)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture1", m_iKey)))
		return E_FAIL;
	if (FAILED(m_pSelectTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture2", 0)))
		return E_FAIL;

	return S_OK;
}

CKeyBord* CKeyBord::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CKeyBord* pInstance = new CKeyBord(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CKeyBord");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CKeyBord::Clone(void* pArg)
{
	CKeyBord* pInstance = new CKeyBord(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CKeyBord");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKeyBord::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pSelectTextureCom);
	Safe_Release(m_pShaderCom);
}
