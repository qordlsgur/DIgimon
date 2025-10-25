#include "Skill_Effect_Image.h"
#include "GameInstance.h"

CSkill_Effect_Image::CSkill_Effect_Image(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEffect{ pDevice, pContext }
{
}

CSkill_Effect_Image::CSkill_Effect_Image(const CSkill_Effect_Image& Prototype)
	: CEffect{ Prototype }
{
}

HRESULT CSkill_Effect_Image::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkill_Effect_Image::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Scale(100.f, 100.f, 100.f);

	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(100.f, 30.f, 500.f,1.f));
	m_fTime = 50.f;

	return S_OK;
}

void CSkill_Effect_Image::Priority_Update(_float fTimeDelta)
{
}

void CSkill_Effect_Image::Update(_float fTimeDelta)
{
	m_fTime -= fTimeDelta * 50.f;

	m_pTransformCom->Set_Scale(m_fTime,m_fTime, m_fTime);

	if (m_fTime < 0.f)
		m_fTime = 50.f;

	if (m_pGameInstance->Key_Down(DIK_L))
		a++;

	if (m_pGameInstance->Key_Down(DIK_K))
		a--;

	if (a < 0)
		a = 5;
	
	if (a > 4)
		a = 0;
}

void CSkill_Effect_Image::Late_Update(_float fTimeDelta)
{
	Compute_Depth();

	m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
	//m_pGameInstance->Add_RenderGroup(RENDER::BLUR, this);

}

HRESULT CSkill_Effect_Image::Render()
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

HRESULT CSkill_Effect_Image::Ready_Components()
{
	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Battle_Diffuse */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Space"),
		TEXT("Com_Battle_Diffuse"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;	
	
	/* Com_Battle_Dissolve*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Noise"),
		TEXT("Com_Battle_Dissolve"), reinterpret_cast<CComponent**>(&m_pDissolveTextureCom))))
		return E_FAIL;	
	
	/* Com_Battle_Mask*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_CircleEffect"),
		TEXT("Com_Battle_Mask"), reinterpret_cast<CComponent**>(&m_pMaskTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkill_Effect_Image::Bind_ShaderResources()
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
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", a)))
		return E_FAIL;
	if (FAILED(m_pDissolveTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Dissolve", 0)))
		return E_FAIL;
	if (FAILED(m_pMaskTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Mask", 1)))
		return E_FAIL;
	return S_OK;
}

CSkill_Effect_Image* CSkill_Effect_Image::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSkill_Effect_Image* pInstance = new CSkill_Effect_Image(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSkill_Effect_Image ");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CSkill_Effect_Image::Clone(void* pArg)
{
	CSkill_Effect_Image* pInstance = new CSkill_Effect_Image(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created :CBattle_Turn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkill_Effect_Image::Free()
{
	__super::Free();

	Safe_Release(m_pDissolveTextureCom);
	Safe_Release(m_pMaskTextureCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
}

