#include "AngewomonSkill1_Part2.h"
#include "GameInstance.h"

CAngewomonSkill1_Part2::CAngewomonSkill1_Part2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CPartObject{ pDevice, pContext }
{
}

CAngewomonSkill1_Part2::CAngewomonSkill1_Part2(const CAngewomonSkill1_Part2& Prototype)
    : CPartObject{ Prototype }
{
}

HRESULT CAngewomonSkill1_Part2::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CAngewomonSkill1_Part2::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_vPosition.m128_f32[1] += 10.f;

    m_pTransformCom->Set_State(STATE::POSITION, m_vPosition);
    m_pTransformCom->Set_Scale(1000.f, 1000.f, 1000.f);
    m_fTime = 50.f;

    return S_OK;
}

void CAngewomonSkill1_Part2::Priority_Update(_float fTimeDelta)
{
}

void CAngewomonSkill1_Part2::Update(_float fTimeDelta)
{
    m_fTime += fTimeDelta * 100.f;

    m_pTransformCom->Set_Scale(m_fTime, m_fTime, m_fTime);

    //if (m_fTime < 10.f)
    //    m_fTime = 50.f;
}

void CAngewomonSkill1_Part2::Late_Update(_float fTimeDelta)
{
    Compute_Depth();

    m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
}

HRESULT CAngewomonSkill1_Part2::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(3)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Resources()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    return S_OK;
}

HRESULT CAngewomonSkill1_Part2::Ready_Components()
{
    /* Com_VIBuffer */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* Com_Battle_Diffuse */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Light"),
        TEXT("Com_Battle_Diffuse"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /* Com_Battle_Mask*/
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Circle"),
        TEXT("Com_Battle_Mask"), reinterpret_cast<CComponent**>(&m_pMaskTextureCom))))
        return E_FAIL;

    /* Com_Shader */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CAngewomonSkill1_Part2::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
        return E_FAIL;
    if (FAILED(m_pGameInstance->Bind_RenderTarget(TEXT("Target_Depth"), m_pShaderCom, "g_DepthTexture")))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_State("Time", m_fTime)))
        return E_FAIL;
    if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 2)))
        return E_FAIL;
    if (FAILED(m_pMaskTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Mask", 0)))
        return E_FAIL;
    return S_OK;
}

CAngewomonSkill1_Part2* CAngewomonSkill1_Part2::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CAngewomonSkill1_Part2* pInstance = new CAngewomonSkill1_Part2(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CAngewomonSkill1_Part1 ");
        Safe_Release(pInstance);
    }

    return pInstance;
}


CGameObject* CAngewomonSkill1_Part2::Clone(void* pArg)
{
    CAngewomonSkill1_Part2* pInstance = new CAngewomonSkill1_Part2(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created :CBattle_Turn");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CAngewomonSkill1_Part2::Free()
{
    __super::Free();

    Safe_Release(m_pMaskTextureCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pShaderCom);
}

