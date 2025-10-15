#include "Body_Beelzebumon.h"
#include "GameInstance.h"

CBody_Beelzebumon::CBody_Beelzebumon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject{ pDevice, pContext }
{
}

CBody_Beelzebumon::CBody_Beelzebumon(const CBody_Beelzebumon& Prototype)
	: CPartObject{ Prototype }
{
}

_float4x4* CBody_Beelzebumon::Get_BoneMatrixPtr(const _char* pBoneName)
{
	return m_pModelCom->Get_BoneMatrixPtr(pBoneName);
}

_bool CBody_Beelzebumon::isFinish_Att()
{
	return false;
}

HRESULT CBody_Beelzebumon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBody_Beelzebumon::Initialize(void* pArg)
{
	BODY_PLAYER_DESC* pDesc = static_cast<BODY_PLAYER_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pModelCom->Set_AnimSpeed("Skill_1", 30);
	m_pModelCom->Set_AnimSpeed("Skill_2", 30);
	m_pModelCom->Set_AnimSpeed("Skill_3", 30);

	return S_OK;
}

void CBody_Beelzebumon::Priority_Update(_float fTimeDelta)
{
}

void CBody_Beelzebumon::Update(_float fTimeDelta)
{
	m_isAnimFinish = m_pModelCom->Play_Animation(fTimeDelta);

	XMStoreFloat4x4(&m_CombinedWorldMatrix,
		XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()) * XMLoadFloat4x4(m_pParentTransformCom->Get_WorldMatrixPtr()));
}

void CBody_Beelzebumon::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CBody_Beelzebumon::Render()
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


		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;

	}

	return S_OK;
}

void CBody_Beelzebumon::Set_Animation(const _char* szName, _bool bisLoop)
{
	m_pModelCom->Set_AnimationIndex(szName, bisLoop);
}

HRESULT CBody_Beelzebumon::Ready_Components()
{
	/* Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Beelzebumon"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CBody_Beelzebumon::Bind_ShaderResources()
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

CBody_Beelzebumon* CBody_Beelzebumon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBody_Beelzebumon* pInstance = new CBody_Beelzebumon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBody_Beelzebumon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBody_Beelzebumon::Clone(void* pArg)
{
	CBody_Beelzebumon* pInstance = new CBody_Beelzebumon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBody_Beelzebumon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBody_Beelzebumon::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}