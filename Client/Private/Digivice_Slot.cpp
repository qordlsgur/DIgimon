#include "Digivice_Slot.h"

#include "GameInstance.h"

CDigivice_Slot::CDigivice_Slot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUIObject{ pDevice, pContext }
{
}

CDigivice_Slot::CDigivice_Slot(const CDigivice_Slot& Prototype)
    : CUIObject{ Prototype }
{
}

HRESULT CDigivice_Slot::Initialize_Prototype()
{
    return E_NOTIMPL;
}

HRESULT CDigivice_Slot::Initialize(void* pArg)
{
    return E_NOTIMPL;
}

void CDigivice_Slot::Priority_Update(_float fTimeDelta)
{
}

void CDigivice_Slot::Update(_float fTimeDelta)
{
}

void CDigivice_Slot::Late_Update(_float fTimeDelta)
{
}

HRESULT CDigivice_Slot::Render()
{
    return E_NOTIMPL;
}

CGameObject* CDigivice_Slot::Clone(void* pArg)
{
    return nullptr;
}

void CDigivice_Slot::Free()
{
}
