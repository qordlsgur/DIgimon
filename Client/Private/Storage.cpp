#include "Storage.h"

#include "GameInstance.h"

CStorage::CStorage(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUIObject{ pDevice, pContext }
{
}

CStorage::CStorage(const CStorage& Prototype)
    : CUIObject{ Prototype }
{
}

HRESULT CStorage::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CStorage::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CStorage::Priority_Update(_float fTimeDelta)
{
}

void CStorage::Update(_float fTimeDelta)
{
}

void CStorage::Late_Update(_float fTimeDelta)
{
}

HRESULT CStorage::Render()
{
    m_pGameInstance->Add_RenderGroup(RENDER::UI, this);

    return S_OK;
}

HRESULT CStorage::Ready_Components()
{
    return S_OK;
}

CStorage* CStorage::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    {
        CStorage* pInstance = new CStorage(pDevice, pContext);

        if (FAILED(pInstance->Initialize_Prototype()))
        {
            MSG_BOX("Failed to Created : CStorage_Prototype ");
            Safe_Release(pInstance);
        }

        return pInstance;
    }

}

CGameObject* CStorage::Clone(void* pArg)
{
    CStorage* pInstance = new CStorage(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CStorage");
        Safe_Release(pInstance);
    }

    return pInstance;
}


void CStorage::Free()
{
    __super::Free();
}
