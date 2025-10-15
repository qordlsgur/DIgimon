#include "Battle_Timeline.h"
#include "GameInstance.h"

CBattle_Timeline::CBattle_Timeline(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CBattle_Timeline::CBattle_Timeline(const CBattle_Timeline& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CBattle_Timeline::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBattle_Timeline::Initialize(void* pArg)
{
	CUIObject::UIOBJECT_DESC	Desc{};

	Desc.fX = 1208.f;
	Desc.fY = 200.f;
	Desc.fSizeX = 144.f;
	Desc.fSizeY = 94.f;

	//Desc.fX = 1220.f;
	//Desc.fY = 200.f;
	//Desc.fSizeX = 120.f;
	//Desc.fSizeY = 74.f;


	m_pRect = { long(Desc.fX - Desc.fSizeX * 0.5f), long(Desc.fY - Desc.fSizeY * 0.5f), long(Desc.fX + Desc.fSizeX * 0.5f), long(Desc.fY + Desc.fSizeY * 0.5f) };

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CBattle_Timeline::Priority_Update(_float fTimeDelta)
{
}

void CBattle_Timeline::Update(_float fTimeDelta)
{
	_vector vWorldPos = XMVectorSet(m_fX - m_fWinSizeX * 0.5f, -m_fY + m_fWinSizeY * 0.5f, 0.f, 1.f);

	m_pTransformCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(STATE::POSITION, vWorldPos);

	_float4 Pos;
	XMStoreFloat4(&Pos, vWorldPos);

	m_pRect = {
		long(m_fX - m_fSizeX * 0.5f),
		long(m_fY - m_fSizeY * 0.5f),
		long(m_fX + m_fSizeX * 0.5f),
		long(m_fY + m_fSizeY * 0.5f)
	};

	if (m_bisLerp)
		Set_MoveUp(m_fOffSet);

	if (m_bisSlowLerp)
		Set_MoveSlowUp(m_fSlowOffSet);

	if (m_bActive == false)
		m_fTime += fTimeDelta;

	if (m_fTime >= 0.5f)
	{
		m_bActive = true;
		m_fTime = 0.f;
	}
}

void CBattle_Timeline::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::UI, this);
}

HRESULT CBattle_Timeline::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Bind_Resources()))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;
	}
	return S_OK;
}

void CBattle_Timeline::Set_Move(_float2 OffSet)
{
	m_fX = OffSet.x;
	m_fY = OffSet.y;
}

void CBattle_Timeline::Set_MoveUp(_float2 OffSet)
{
	if (m_fY >= OffSet.y)
	{
		m_fY -= 17.f;
	}

	if (m_fY <= 234)
	{
		Set_SizeUp();
		m_fX = OffSet.x;
	}

	if (m_fY <= OffSet.y)
	{
		m_fY = OffSet.y;
		m_bisLerp = false;
	}
}

void CBattle_Timeline::Set_MoveSlowUp(_float2 OffSet)
{
	if (m_fY >= OffSet.y)
	{
		m_fY -= 15.f;
	}

	if (m_fY <= OffSet.y)
	{
		m_fY = OffSet.y;
		m_bisSlowLerp = false;
	}
}

void CBattle_Timeline::Set_SizeUp()
{
	m_fSizeX = 144.f;
	m_fSizeY = 94.f;
}

void CBattle_Timeline::Set_SizeDown()
{
	m_fSizeX = 120.f;
	m_fSizeY = 74.f;
}

HRESULT CBattle_Timeline::Ready_Components()
{
	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Battle_TimeLine*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Timeline_Portrait_mask"),
		TEXT("Com_Battle_TimeLine"), reinterpret_cast<CComponent**>(&m_pTimeLine_TextureCom))))
		return E_FAIL;
	/* Com_BlueBG*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Timeline_bg"),
		TEXT("Com_BlueBG"), reinterpret_cast<CComponent**>(&m_pTimelineBGTextureCom))))
		return E_FAIL;
	/* Com_BlueLine*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Timeline_Line"),
		TEXT("Com_BlueLine"), reinterpret_cast<CComponent**>(&m_pBattleTimeLineLineTextureCom))))
		return E_FAIL;
	/* Com_Battle_Digimon_Texter*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Digimon_Digivice"),
		TEXT("Com_Battle_Digimon_Texter"), reinterpret_cast<CComponent**>(&m_pDigimonTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_Battle_Timeline"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CBattle_Timeline::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pTimeLine_TextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture1", 0)))
		return E_FAIL;
	if (FAILED(m_pTimelineBGTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture2", m_iEnemy)))
		return E_FAIL;
	if (FAILED(m_pDigimonTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture3", m_iDigimon_Image)))
		return E_FAIL;
	if (FAILED(m_pBattleTimeLineLineTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture4", m_iEnemy)))
		return E_FAIL;

	return S_OK;
}

CBattle_Timeline* CBattle_Timeline::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBattle_Timeline* pInstance = new CBattle_Timeline(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDigivice_Skill ");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CBattle_Timeline::Clone(void* pArg)
{
	CBattle_Timeline* pInstance = new CBattle_Timeline(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created :CDigivice_Skill");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBattle_Timeline::Free()
{
	__super::Free();

	Safe_Release(m_pTimeLine_TextureCom);
	Safe_Release(m_pTimelineBGTextureCom);
	Safe_Release(m_pBattleTimeLineLineTextureCom);
	Safe_Release(m_pDigimonTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
}
