#include "LeomonSkill2_Part2.h"
#include "GameInstance.h"

CLeomonSkill2_Part2::CLeomonSkill2_Part2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject{ pDevice, pContext }
{
}

CLeomonSkill2_Part2::CLeomonSkill2_Part2(const CLeomonSkill2_Part2& Prototype)
	: CPartObject{ Prototype }
{
}

HRESULT CLeomonSkill2_Part2::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLeomonSkill2_Part2::Initialize(void* pArg)
{
	BODY_PLAYER_DESC* pDesc = static_cast<BODY_PLAYER_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	XMStoreFloat4x4(&m_CombinedWorldMatrix,
		XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()) * XMLoadFloat4x4(m_pParentTransformCom->Get_WorldMatrixPtr()));

	m_fTime = 10.f;


	m_pTransformCom->Set_State(STATE::POSITION, m_vPosition);
	m_pTransformCom->LookAtCamera(XMLoadFloat4(m_pGameInstance->Get_CamPosition()), 10.f, 0.0016);

	m_vPosition = XMLoadFloat4x4(&m_CombinedWorldMatrix).r[3];

	return S_OK;
}


void CLeomonSkill2_Part2::Priority_Update(_float fTimeDelta)
{
}

void CLeomonSkill2_Part2::Update(_float fTimeDelta)
{
	m_fTime -= fTimeDelta * 15.f;

	m_pTransformCom->Set_Scale(m_fTime, m_fTime, m_fTime);

	m_pTransformCom->Set_State(STATE::POSITION, m_vPosition);

	m_pTransformCom->LookAtCamera(XMLoadFloat4(m_pGameInstance->Get_CamPosition()), 10.f, fTimeDelta);
}

void CLeomonSkill2_Part2::Late_Update(_float fTimeDelta)
{
	if (!m_bHit)
	{
		m_pGameInstance->Add_RenderGroup(RENDER::EFFECT, this);
		m_pGameInstance->Add_RenderGroup(RENDER::BLUR, this);
	}
}

HRESULT CLeomonSkill2_Part2::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(1)))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(0)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLeomonSkill2_Part2::Ready_Components()
{
	/* Com_Mode */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Plane"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_CircleEffect"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_MetalgreymonSkill3_Image"),
		TEXT("Com_Texture1"), reinterpret_cast<CComponent**>(&m_pTexture1Com))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxSkillLeomon"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLeomonSkill2_Part2::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_State("Time", m_fTime)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 9)))
		return E_FAIL;
	if (FAILED(m_pTexture1Com->Bind_ShaderResource(m_pShaderCom, "g_Mask", 1)))
		return E_FAIL;


	return S_OK;
}
CLeomonSkill2_Part2* CLeomonSkill2_Part2::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLeomonSkill2_Part2* pInstance = new CLeomonSkill2_Part2(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLeomonSkill2_Part2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLeomonSkill2_Part2::Clone(void* pArg)
{
	CLeomonSkill2_Part2* pInstance = new CLeomonSkill2_Part2(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CLeomonSkill2_Part2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLeomonSkill2_Part2::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}

