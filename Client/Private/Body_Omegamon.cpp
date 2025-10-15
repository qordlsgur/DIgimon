#include "Body_Omegamon.h"
#include "GameInstance.h"


CBody_Omegamon::CBody_Omegamon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CPartObject{ pDevice, pContext }
{
}

CBody_Omegamon::CBody_Omegamon(const CBody_Omegamon& Prototype)
    : CPartObject{ Prototype }
{
}

_float4x4* CBody_Omegamon::Get_BoneMatrixPtr(const _char* pBoneName)
{
    return m_pModelCom->Get_BoneMatrixPtr(pBoneName);
}

_bool CBody_Omegamon::isFinish_Att()
{
    return false;
}

HRESULT CBody_Omegamon::Initialize_Prototype()
{
    return S_OK;

}

HRESULT CBody_Omegamon::Initialize(void* pArg)
{
    BODY_PLAYER_DESC* pDesc = static_cast<BODY_PLAYER_DESC*>(pArg);

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pModelCom->Set_AnimSpeed("BattleBack", 20.3f);
    m_pModelCom->Set_AnimSpeed("Skill_1", 30);
    m_pModelCom->Set_AnimSpeed("Skill_2", 35);
    m_pModelCom->Set_AnimSpeed("Skill_3", 30);

    return S_OK;
}

void CBody_Omegamon::Priority_Update(_float fTimeDelta)
{
}

void CBody_Omegamon::Update(_float fTimeDelta)
{
    m_isAnimFinish = m_pModelCom->Play_Animation(fTimeDelta);

    Canon();

    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()) * XMLoadFloat4x4(m_pParentTransformCom->Get_WorldMatrixPtr()));
}

void CBody_Omegamon::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CBody_Omegamon::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_BoneMatrices(i, m_pShaderCom, "g_BoneMatrices")))
            return E_FAIL;

        if (FAILED(m_pModelCom->Bind_Material(i, m_pShaderCom, "g_DiffuseTexture", 0)))
            return E_FAIL;
        /*if (FAILED(m_pModelCom->Bind_Material(i, m_pShaderCom, "g_DiffuseTexture", aiTextureType_DIFFUSE, 0)))
            return E_FAIL;*/

        if (FAILED(m_pShaderCom->Begin(0)))
            return E_FAIL;


        if (FAILED(m_pModelCom->Render(0)))
            return E_FAIL;
        if (isCanon)
        {
            if (FAILED(m_pModelCom->Render(1)))
                return E_FAIL;
        }
    }

    return S_OK;
}

void CBody_Omegamon::Set_Animation(const _char* szName, _bool bisLoop)
{
    m_pModelCom->Set_AnimationIndex(szName, bisLoop);
}

void CBody_Omegamon::Canon()
{
    if (m_eState == DIGIMONSTATE::SKILL3)
    {
        if (m_pModelCom->Get_CurrentTrackPosition() >= 18)
            isCanon = true;

        if (m_pModelCom->Get_CurrentTrackPosition() > 105)
            isCanon = false;
    }
}

HRESULT CBody_Omegamon::Ready_Components()
{
    /* Com_Model */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Omegamon"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    /* Com_Shader */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CBody_Omegamon::Bind_ShaderResources()
{
    /*m_pShaderCom->Bind_Matrix("g_WorldMatrix", );*/
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
        return E_FAIL;




    return S_OK;
}

CBody_Omegamon* CBody_Omegamon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBody_Omegamon* pInstance = new CBody_Omegamon(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CBody_Omegamon");
        Safe_Release(pInstance);
    }
        
    return pInstance;
}

CGameObject* CBody_Omegamon::Clone(void* pArg)
{
    CBody_Omegamon* pInstance = new CBody_Omegamon(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CBody_Omegamon");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBody_Omegamon::Free()
{
    __super::Free();

    Safe_Release(m_pModelCom);
    Safe_Release(m_pShaderCom);
}
