#include "DigiDex.h"

#include "GameInstance.h"
#include "DigiDex_Manager.h"
CDigiDex::CDigiDex(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUIObject{ pDevice, pContext }
{
}

CDigiDex::CDigiDex(const CDigiDex& Prototype)
    : CUIObject{ Prototype }
{
}

HRESULT CDigiDex::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CDigiDex::Initialize(void* pArg)
{
    CUIObject::UIOBJECT_DESC	Desc{};

    Desc.fX = 700.f;
    Desc.fY = 400.f;
    Desc.fSizeX = 884.f;
    Desc.fSizeY = 462.f;

    m_pRect = { long(Desc.fX - Desc.fSizeX * 0.5f), long(Desc.fY - Desc.fSizeY * 0.5f), long(Desc.fX + Desc.fSizeX * 0.5f), long(Desc.fY + Desc.fSizeY * 0.5f) };

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;


    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pDigiDex_Manager = CDigiDex_Manager::GetInstance();
    m_pDigiDex_Manager->DigiDex(this);

    return S_OK;
}

void CDigiDex::Priority_Update(_float fTimeDelta)
{
}

void CDigiDex::Update(_float fTimeDelta)
{
    if (m_pGameInstance->Key_Down(DIK_U))
        Set_Active();
}

void CDigiDex::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::UI, this);
}

HRESULT CDigiDex::Render()
{
    if (m_bActive)
    {
        if (FAILED(Bind_ShaderResources()))
            return E_FAIL;

        if (FAILED(m_pShaderCom->Begin(0)))
            return E_FAIL;

        if (FAILED(m_pVIBufferCom->Bind_Resources()))
            return E_FAIL;

        __super::Begin();
        __super::Blend_Begin();

        if (FAILED(m_pVIBufferCom->Render()))
            return E_FAIL;

        __super::End();
        __super::Blend_End();
    }
    return S_OK;
}

HRESULT CDigiDex::Ready_Components()
{
    /* Com_VIBuffer */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_DigiDex"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /* Com_Shader */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_Inventory"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;
    return S_OK;
}

HRESULT CDigiDex::Bind_ShaderResources()
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

CDigiDex* CDigiDex::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    {
        CDigiDex* pInstance = new CDigiDex(pDevice, pContext);

        if (FAILED(pInstance->Initialize_Prototype()))
        {
            MSG_BOX("Failed to Created : DigiDex_Prototype ");
            Safe_Release(pInstance);
        }

        return pInstance;
    }

}

CGameObject* CDigiDex::Clone(void* pArg)
{

    CDigiDex* pInstance = new CDigiDex(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CDigiDex");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CDigiDex::Free()
{
    __super::Free();	
    
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pShaderCom);
}