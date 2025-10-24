#include "Effect.h"
#include "GameInstance.h"

CEffect::CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CGameObject{ pDevice, pContext }
{
}

CEffect::CEffect(const CEffect& Prototype)
    : CGameObject{ Prototype }
{
}

HRESULT CEffect::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CEffect::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CEffect::Priority_Update(_float fTimeDelta)
{
}

void CEffect::Update(_float fTimeDelta)
{
}

void CEffect::Late_Update(_float fTimeDelta)
{
}

HRESULT CEffect::Render()
{
    return S_OK;
}

void CEffect::Compute_Depth()
{
    _vector		vCamPosition = XMLoadFloat4(m_pGameInstance->Get_CamPosition());

    m_fDepth = XMVectorGetX(XMVector3Length(vCamPosition - m_pTransformCom->Get_State(STATE::POSITION)));
}

void CEffect::Free()
{
    __super::Free();

        
}
