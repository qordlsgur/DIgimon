#include "Battle_Enemy_HpBar.h"
#include "GameInstance.h"

CBattle_Enemy_HpBar::CBattle_Enemy_HpBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CBattle_Enemy_HpBar::CBattle_Enemy_HpBar(const CBattle_Enemy_HpBar& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CBattle_Enemy_HpBar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBattle_Enemy_HpBar::Initialize(void* pArg)
{
	CUIObject::UIOBJECT_DESC	Desc{};

	Desc.fX = 0.f;
	Desc.fY = 0.f;
	Desc.fSizeX = 168.f;
	Desc.fSizeY = 18.f;

	m_pRect = { long(Desc.fX - Desc.fSizeX * 0.5f), long(Desc.fY - Desc.fSizeY * 0.5f), long(Desc.fX + Desc.fSizeX * 0.5f), long(Desc.fY + Desc.fSizeY * 0.5f) };

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CBattle_Enemy_HpBar::Priority_Update(_float fTimeDelta)
{
}

void CBattle_Enemy_HpBar::Update(_float fTimeDelta)
{
	_vector vWorldPos = XMVectorSet(m_fX + m_fParent_WorldPos.x, -(m_fY)+m_fParent_WorldPos.y, 0.f, 1.f);

	m_pTransformCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(STATE::POSITION, vWorldPos);

	
	XMStoreFloat4(&Pos, vWorldPos);

	m_pRect = {
		long(m_fX - m_fSizeX * 0.5f),
		long(m_fY - m_fSizeY * 0.5f),
		long(m_fX + m_fSizeX * 0.5f),
		long(m_fY + m_fSizeY * 0.5f)
	};

	m_fHpRatio = m_fCurrentHp / m_fMaxHp;

	if (m_fHpRatio < 0.f)
		m_fHpRatio = 0.f;
}

void CBattle_Enemy_HpBar::Late_Update(_float fTimeDelta)
{
}

HRESULT CBattle_Enemy_HpBar::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;
	
	return S_OK;
}

void CBattle_Enemy_HpBar::Set_Parent_WorldPos(_vector fParent_World)
{
	XMStoreFloat4(&m_fParent_WorldPos, fParent_World);
}

void CBattle_Enemy_HpBar::Set_MaxHp(_float MaxHp)
{
	m_fMaxHp = MaxHp;
	m_fCurrentHp = MaxHp;
}

void CBattle_Enemy_HpBar::Set_Damage(_int Damage)
{
	m_fCurrentHp -= Damage;
}

void CBattle_Enemy_HpBar::Set_Move(_float fX, _float fY)
{
	m_fX = fX;
	m_fY = fY;
}


HRESULT CBattle_Enemy_HpBar::Ready_Components()
{
	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Battle_Hp*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Battle_Rotation_Gauge_HP"),
		TEXT("Com_Battle_Hp"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_Battle_Hp_BG*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Battle_Rotation_Gauge_BG"),
		TEXT("Com_Battle_Hp_BG"), reinterpret_cast<CComponent**>(&m_pBgTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_Digivice_State"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CBattle_Enemy_HpBar::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_State("Current", m_fHpRatio)))
		return E_FAIL;
	if (FAILED(m_pBgTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture1", 0)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture2", 0)))
		return E_FAIL;

	return S_OK;
}

CBattle_Enemy_HpBar* CBattle_Enemy_HpBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBattle_Enemy_HpBar* pInstance = new CBattle_Enemy_HpBar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBattle_Enemy_HpBar ");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CBattle_Enemy_HpBar::Clone(void* pArg)
{
	CBattle_Enemy_HpBar* pInstance = new CBattle_Enemy_HpBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created :CBattle_Enemy_HpBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBattle_Enemy_HpBar::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pBgTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
}
