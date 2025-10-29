#include "BlackwargreymonSkill3_Part1.h"
#include "GameInstance.h"

CBlackwargreymonSkill3_Part1::CBlackwargreymonSkill3_Part1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject{ pDevice, pContext }
{
}

CBlackwargreymonSkill3_Part1::CBlackwargreymonSkill3_Part1(const CBlackwargreymonSkill3_Part1& Prototype)
	: CPartObject{ Prototype }
{
}

_float4x4* CBlackwargreymonSkill3_Part1::Get_BoneMatrixPtr(const _char* pBoneName)
{
	return nullptr;
}

HRESULT CBlackwargreymonSkill3_Part1::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBlackwargreymonSkill3_Part1::Initialize(void* pArg)
{
	BODY_PLAYER_DESC* pDesc = static_cast<BODY_PLAYER_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CBlackwargreymonSkill3_Part1::Priority_Update(_float fTimeDelta)
{
}

void CBlackwargreymonSkill3_Part1::Update(_float fTimeDelta)
{
	XMStoreFloat4x4(&m_CombinedWorldMatrix,
		XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()) * XMLoadFloat4x4(m_pParentTransformCom->Get_WorldMatrixPtr()));
}

void CBlackwargreymonSkill3_Part1::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
	//m_pGameInstance->Add_RenderGroup(RENDER::BLUR, this);
}

HRESULT CBlackwargreymonSkill3_Part1::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(0)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBlackwargreymonSkill3_Part1::Ready_Components()
{
	/* Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_WargreymonSkill2_1"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_WargreymonSkill2_Image"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxSkillBlackwargreymon"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CBlackwargreymonSkill3_Part1::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	return S_OK;
}

CBlackwargreymonSkill3_Part1* CBlackwargreymonSkill3_Part1::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBlackwargreymonSkill3_Part1* pInstance = new CBlackwargreymonSkill3_Part1(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBlackwargreymonSkill3_Part1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBlackwargreymonSkill3_Part1::Clone(void* pArg)
{
	CBlackwargreymonSkill3_Part1* pInstance = new CBlackwargreymonSkill3_Part1(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBlackwargreymonSkill3_Part1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBlackwargreymonSkill3_Part1::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}

