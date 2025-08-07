#include "PartyUHD.h"

CPartyUHD::CPartyUHD(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUIObject{pDevice, pContext}
{
}

CPartyUHD::CPartyUHD(const CPartyUHD& Prototype)
    : CUIObject{Prototype}
{
}

HRESULT CPartyUHD::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CPartyUHD::Initialize(void* pArg)
{
    return S_OK;
}

void CPartyUHD::Priority_Update(_float fTimeDelta)
{
}

void CPartyUHD::Update(_float fTimeDelta)
{
}

void CPartyUHD::Late_Update(_float fTimeDelta)
{
}

HRESULT CPartyUHD::Render()
{
    return S_OK;
}

HRESULT CPartyUHD::Ready_Components()
{
    return S_OK;
}

HRESULT CPartyUHD::Bind_ShaderResources()
{
    return S_OK;
}

HRESULT CPartyUHD::Create_Slot(const _wstring& strLayerTag)
{
    return S_OK;
}

CPartyUHD* CPartyUHD::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPartyUHD* pInstance = new CPartyUHD(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPartyUHD ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPartyUHD::Clone(void* pArg)
{
	CPartyUHD* pInstance = new CPartyUHD(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created :CPartyUHD");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPartyUHD::Free()
{
	__super::Free();

	//Safe_Release(m_pVIBufferCom);
	//Safe_Release(m_pTextureCom);
	//Safe_Release(m_pShaderCom);
}

