#include "MetalgreymonSkill3_Part1.h"
#include "GameInstance.h"

CMetalgreymonSkill3_Part1::CMetalgreymonSkill3_Part1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject{ pDevice, pContext }
{
}

CMetalgreymonSkill3_Part1::CMetalgreymonSkill3_Part1(const CMetalgreymonSkill3_Part1& Prototype)
	: CPartObject{ Prototype }
{
}

_float4x4* CMetalgreymonSkill3_Part1::Get_BoneMatrixPtr(const _char* pBoneName)
{
	return m_pModelCom->Get_BoneMatrixPtr(pBoneName);
}

HRESULT CMetalgreymonSkill3_Part1::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMetalgreymonSkill3_Part1::Initialize(void* pArg)
{
	BODY_PLAYER_DESC* pDesc = static_cast<BODY_PLAYER_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Scale(3.f, 3.f, 3.f);
	m_mHatch = pDesc->vMatrix;

	return S_OK;
}

void CMetalgreymonSkill3_Part1::Priority_Update(_float fTimeDelta)
{
}

void CMetalgreymonSkill3_Part1::Update(_float fTimeDelta)
{
	XMStoreFloat4x4(&m_CombinedWorldMatrix,
		XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()) * m_mHatch * XMLoadFloat4x4(m_pParentTransformCom->Get_WorldMatrixPtr()));

	m_fTime += fTimeDelta * 5.f;
}


void CMetalgreymonSkill3_Part1::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::EFFECT, this);
	m_pGameInstance->Add_RenderGroup(RENDER::BLUR, this);
}

HRESULT CMetalgreymonSkill3_Part1::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(3)))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(0)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMetalgreymonSkill3_Part1::Ready_Components()
{
	/* Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Metalgarumon_Skill3"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_MetalgreymonSkill3_Image"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_MaskTexture*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_MetalgreymonSkill3_Image"),
		TEXT("Com_MaskTexture"), reinterpret_cast<CComponent**>(&m_pMaskTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxSkillMetalgarumon"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMetalgreymonSkill3_Part1::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_State("Time", m_fTime)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 1)))
		return E_FAIL;
	if (FAILED(m_pMaskTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Mask",1)))
		return E_FAIL;

	return S_OK;
}

CMetalgreymonSkill3_Part1* CMetalgreymonSkill3_Part1::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMetalgreymonSkill3_Part1* pInstance = new CMetalgreymonSkill3_Part1(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMetalgreymonSkill3_Part1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMetalgreymonSkill3_Part1::Clone(void* pArg)
{
	CMetalgreymonSkill3_Part1* pInstance = new CMetalgreymonSkill3_Part1(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMetalgreymonSkill3_Part1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMetalgreymonSkill3_Part1::Free()
{
	__super::Free();

	Safe_Release(m_pMaskTextureCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
