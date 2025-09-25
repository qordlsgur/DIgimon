#include "DigimonInfo.h"
#include "GameInstance.h"

CDigimonInfo::CDigimonInfo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CDigimonInfo::CDigimonInfo(const CDigimonInfo& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CDigimonInfo::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CDigimonInfo::Initialize(void* pArg)
{
    return S_OK;
}

void CDigimonInfo::Priority_Update(_float fTimeDelta)
{

}

void CDigimonInfo::Update(_float fTimeDelta)
{

}

void CDigimonInfo::Late_Update(_float fTimeDelta)
{

}

HRESULT CDigimonInfo::Render()
{
	return S_OK;
}

CDigimonInfo* CDigimonInfo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDigimonInfo* pInstance = new CDigimonInfo(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDigimonInfo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDigimonInfo::Clone(void* pArg)
{
	CDigimonInfo* pInstance = new CDigimonInfo(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWargreymon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDigimonInfo::Free()
{
	__super::Free();
}
