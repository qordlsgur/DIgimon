#include "Battle_Turn.h"
#include "GameInstance.h"

CBattle_Turn::CBattle_Turn(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CBattle_Turn::CBattle_Turn(const CBattle_Turn& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CBattle_Turn::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBattle_Turn::Initialize(void* pArg)
{
	CUIObject::UIOBJECT_DESC	Desc{};

	Desc.fX = 1180;
	Desc.fY = 400.f;
	Desc.fSizeX = 200.f;
	Desc.fSizeY = 570.f;

	m_pRect = { long(Desc.fX - Desc.fSizeX * 0.5f), long(Desc.fY - Desc.fSizeY * 0.5f), long(Desc.fX + Desc.fSizeX * 0.5f), long(Desc.fY + Desc.fSizeY * 0.5f) };

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CBattle_Turn::Priority_Update(_float fTimeDelta)
{
}

void CBattle_Turn::Update(_float fTimeDelta)
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
}

void CBattle_Turn::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::UI, this);
}

HRESULT CBattle_Turn::Render()
{
	if (m_bActive)
	{

		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Bind_Resources()))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;
	}

	return S_OK;
}

void CBattle_Turn::Set_Penel()
{
}

HRESULT CBattle_Turn::Ready_Components()
{
	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Battle_TimeLine*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Battle_Turn"),
		TEXT("Com_Battle_TimeLine"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_Digivice_Target"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}


HRESULT CBattle_Turn::Bind_ShaderResources()
{
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

CBattle_Turn* CBattle_Turn::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBattle_Turn* pInstance = new CBattle_Turn(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBattle_Turn ");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CBattle_Turn::Clone(void* pArg)
{
	CBattle_Turn* pInstance = new CBattle_Turn(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created :CBattle_Turn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBattle_Turn::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
}

