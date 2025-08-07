#include "Exit_Button.h"

#include "GameInstance.h"

CExit_Button::CExit_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CButton{pDevice, pContext}
{
}

CExit_Button::CExit_Button(const CExit_Button& Prototype)
    : CButton{ Prototype }
{
}

HRESULT CExit_Button::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CExit_Button::Initialize(void* pArg)
{
	CButton::BUTTON_DESC	Desc{};

	Desc.fX = 100.f;
	Desc.fY = 100.f;
	Desc.fSizeX = 70.f;
	Desc.fSizeY = 70.f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;


	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CExit_Button::Priority_Update(_float fTimeDelta)
{
}

void CExit_Button::Update(_float fTimeDelta)
{
}

void CExit_Button::Late_Update(_float fTimeDelta)
{
	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_fX - m_fSizeX * 0.5f, -m_fY + m_fSizeY * 0.5f, 0.f, 1.f) + XMLoadFloat4(&m_fParent_WorldPos));
}


HRESULT CExit_Button::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	//__super::Begin();

	D3D11_BLEND_DESC pDesc{};

	pDesc.AlphaToCoverageEnable = false;
	pDesc.IndependentBlendEnable = false;

	pDesc.RenderTarget[0].BlendEnable = true;
	pDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	pDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	pDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	pDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	pDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	pDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	pDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	ID3D11BlendState* BS = nullptr;

	m_pDevice->CreateBlendState(&pDesc, &BS);

	_float fBlendFactor[4] = { 0.f,0.f,0.f,0.f };
	_uint iSampleMask = 0xffffffff;
	m_pContext->OMSetBlendState(BS, fBlendFactor, iSampleMask);

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	m_pContext->OMSetBlendState(nullptr, nullptr, iSampleMask);
	Safe_Release(BS);

	//__super::End();

	return S_OK;
}


void CExit_Button::Set_Parent_WorldPos(_vector fParent_World)
{
	XMStoreFloat4(&m_fParent_WorldPos, fParent_World);
}

void CExit_Button::Set_Move(_float fX, _float fY)
{
	m_fX = fX;
	m_fY = fY;

	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_fX - m_fSizeX * 0.5f, -m_fY + m_fSizeY * 0.5f, 0.f, 1.f) + XMLoadFloat4(&m_fParent_WorldPos));

}

HRESULT CExit_Button::Ready_Components()
{
	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Exit_Button"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CExit_Button::Bind_ShaderResources()
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

CExit_Button* CExit_Button::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CExit_Button* pInstance = new CExit_Button(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CExit_Button ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CExit_Button::Clone(void* pArg)
{
	CExit_Button* pInstance = new CExit_Button(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created :CExit_Button");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CExit_Button::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}