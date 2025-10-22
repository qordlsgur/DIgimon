#include "Body_Metalgarumon.h"
#include "GameInstance.h"

CBody_Metalgarumon::CBody_Metalgarumon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CPartObject{ pDevice, pContext }
{
}

CBody_Metalgarumon::CBody_Metalgarumon(const CBody_Metalgarumon& Prototype)
    : CPartObject{ Prototype }
{
}

_float4x4* CBody_Metalgarumon::Get_BoneMatrixPtr(const _char* pBoneName)
{
    return m_pModelCom->Get_BoneMatrixPtr(pBoneName);
}

_bool CBody_Metalgarumon::isFinish_Att()
{
    return false;
}

HRESULT CBody_Metalgarumon::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBody_Metalgarumon::Initialize(void* pArg)
{
	BODY_PLAYER_DESC* pDesc = static_cast<BODY_PLAYER_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pModelCom->Set_AnimSpeed("Skill_1", 30);
	m_pModelCom->Set_AnimSpeed("Skill_2", 35);
	m_pModelCom->Set_AnimSpeed("Skill_3", 30);

	return S_OK;
}

void CBody_Metalgarumon::Priority_Update(_float fTimeDelta)
{
}

void CBody_Metalgarumon::Update(_float fTimeDelta)
{
	m_isAnimFinish = m_pModelCom->Play_Animation(fTimeDelta);

	XMStoreFloat4x4(&m_CombinedWorldMatrix,
		XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()) * XMLoadFloat4x4(m_pParentTransformCom->Get_WorldMatrixPtr()));

	if (m_bDissolve)
		m_fTime += fTimeDelta / 3.f;
}

void CBody_Metalgarumon::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CBody_Metalgarumon::Render()
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

		if (m_bDissolve)
		{
			if (FAILED(m_pShaderCom->Begin(1)))
				return E_FAIL;
		}

		else
		{
			if (FAILED(m_pShaderCom->Begin(0)))
				return E_FAIL;
		}



		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;

	}

	return S_OK;
}

void CBody_Metalgarumon::Set_Animation(const _char* szName, _bool bisLoop)
{
	m_pModelCom->Set_AnimationIndex(szName, bisLoop);
}

HRESULT CBody_Metalgarumon::Ready_Components()
{
	/* Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Metalgarumon"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Battle_Dissolve*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Noise"),
		TEXT("Com_Battle_Dissolve"), reinterpret_cast<CComponent**>(&m_pDissolveTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CBody_Metalgarumon::Bind_ShaderResources()
{
	/*m_pShaderCom->Bind_Matrix("g_WorldMatrix", );*/
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	if (FAILED(m_pDissolveTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DessolveTexture", 0)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_State("DissolveTime", m_fTime)))
		return E_FAIL;


	return S_OK;
}

CBody_Metalgarumon* CBody_Metalgarumon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBody_Metalgarumon* pInstance = new CBody_Metalgarumon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBody_Metalgarumon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBody_Metalgarumon::Clone(void* pArg)
{
	CBody_Metalgarumon* pInstance = new CBody_Metalgarumon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBody_Metalgarumon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBody_Metalgarumon::Free()
{
	__super::Free();

	Safe_Release(m_pDissolveTextureCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}