#include "MetalgreymonSkill2_Part2.h"
#include "GameInstance.h"

CMetalgreymonSkill2_Part2::CMetalgreymonSkill2_Part2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject{ pDevice, pContext }
{
}

CMetalgreymonSkill2_Part2::CMetalgreymonSkill2_Part2(const CMetalgreymonSkill2_Part2& Prototype)
	: CPartObject{ Prototype }
{
}

_float4x4* CMetalgreymonSkill2_Part2::Get_BoneMatrixPtr(const _char* pBoneName)
{
	return nullptr;
}

HRESULT CMetalgreymonSkill2_Part2::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMetalgreymonSkill2_Part2::Initialize(void* pArg)
{
	BODY_PLAYER_DESC* pDesc = static_cast<BODY_PLAYER_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Scale(5.f, 5.f, 5.f);
	m_fTime = 0.f;

	m_vTargetPos = pDesc->vPosition;

	XMStoreFloat4x4(&m_CombinedWorldMatrix,
		XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()) * XMLoadFloat4x4(m_pParentTransformCom->Get_WorldMatrixPtr()));
	
	m_vPosition = XMLoadFloat4x4(&m_CombinedWorldMatrix).r[3];

	return S_OK;
}

void CMetalgreymonSkill2_Part2::Priority_Update(_float fTimeDelta)
{
}

void CMetalgreymonSkill2_Part2::Update(_float fTimeDelta)
{
	m_fTime += fTimeDelta * 5.f;

	m_pTransformCom->Set_State(STATE::POSITION, m_vPosition);

}

void CMetalgreymonSkill2_Part2::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
}

HRESULT CMetalgreymonSkill2_Part2::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(4)))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(0)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMetalgreymonSkill2_Part2::Ready_Components()
{
	/* Com_Mode */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Plane"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Fire"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxSkillMetalgarumon"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMetalgreymonSkill2_Part2::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_State("Time", m_fTime)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_State("Count", 8)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_State("Frame", 0.2f)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 2)))
		return E_FAIL;

	return S_OK;
}

CMetalgreymonSkill2_Part2* CMetalgreymonSkill2_Part2::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMetalgreymonSkill2_Part2* pInstance = new CMetalgreymonSkill2_Part2(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMetalgreymonSkill2_Part2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMetalgreymonSkill2_Part2::Clone(void* pArg)
{
	CMetalgreymonSkill2_Part2* pInstance = new CMetalgreymonSkill2_Part2(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMetalgreymonSkill2_Part2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMetalgreymonSkill2_Part2::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}

