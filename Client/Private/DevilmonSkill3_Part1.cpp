#include "DevilmonSkill3_Part1.h"
#include "GameInstance.h"

CDevilmonSkill3_Part1::CDevilmonSkill3_Part1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject{ pDevice, pContext }
{
}

CDevilmonSkill3_Part1::CDevilmonSkill3_Part1(const CDevilmonSkill3_Part1& Prototype)
	: CPartObject{ Prototype }
{
}

_float4x4* CDevilmonSkill3_Part1::Get_BoneMatrixPtr(const _char* pBoneName)
{
	return nullptr;
}

HRESULT CDevilmonSkill3_Part1::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDevilmonSkill3_Part1::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC	Desc{};
	Desc.fRotationPerSec = XMConvertToRadians(180.0f);
	Desc.fSpeedPerSec = 30.f;

	BODY_PLAYER_DESC* pDesc = static_cast<BODY_PLAYER_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Scale(5.f, 5.f, 5.f);

	m_fTime = 1.f;
	return S_OK;
}

void CDevilmonSkill3_Part1::Priority_Update(_float fTimeDelta)
{
}

void CDevilmonSkill3_Part1::Update(_float fTimeDelta)
{
	m_fTime -= fTimeDelta* 3.5f;

	if (m_fTime <= 0.f)
		m_pTransformCom->TurnY(fTimeDelta, 10.f, fTimeDelta);

	XMStoreFloat4x4(&m_CombinedWorldMatrix,
		XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()) * XMLoadFloat4x4(m_pParentTransformCom->Get_WorldMatrixPtr()));

	m_fDethTime += fTimeDelta;

	if (m_fDethTime >= 5.f)
		Set_isDead(true);

}

void CDevilmonSkill3_Part1::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
	m_pGameInstance->Add_RenderGroup(RENDER::BLUR, this);
}

HRESULT CDevilmonSkill3_Part1::Render()
{
	if (!m_isDead)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(0)))
			return E_FAIL;

	}
	
	return S_OK;
}

HRESULT CDevilmonSkill3_Part1::Ready_Components()
{
	/* Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Disc"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_DevilmonSkill3_Image"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Noise"),
		TEXT("Com_DissolveTexture"), reinterpret_cast<CComponent**>(&m_pDissolveTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxSkillDevilmon"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CDevilmonSkill3_Part1::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_State("Time", m_fTime)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	if (FAILED(m_pDissolveTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Dissolve", 9)))
		return E_FAIL;

	return S_OK;
}

CDevilmonSkill3_Part1* CDevilmonSkill3_Part1::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDevilmonSkill3_Part1* pInstance = new CDevilmonSkill3_Part1(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDevilmonSkill3_Part1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDevilmonSkill3_Part1::Clone(void* pArg)
{
	CDevilmonSkill3_Part1* pInstance = new CDevilmonSkill3_Part1(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDevilmonSkill3_Part1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDevilmonSkill3_Part1::Free()
{
	__super::Free();

	Safe_Release(m_pDissolveTextureCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}


