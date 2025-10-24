#include "Battle_Enemy_Hp_BG.h"
#include "GameInstance.h"
#include "Battle_Enemy_HpBar.h"
#include "ContainerObject.h"
#include "Battle_UI_Manager.h"

CBattle_Enemy_Hp_BG::CBattle_Enemy_Hp_BG(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CBattle_Enemy_Hp_BG::CBattle_Enemy_Hp_BG(const CBattle_Enemy_Hp_BG& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CBattle_Enemy_Hp_BG::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBattle_Enemy_Hp_BG::Initialize(void* pArg)
{
	CUIObject::UIOBJECT_DESC	Desc{};

	Desc.fX = 450.f;
	Desc.fY = 50.f;
	Desc.fSizeX = 200.f;
	Desc.fSizeY = 75.f;

	m_pRect = { long(Desc.fX - Desc.fSizeX * 0.5f), long(Desc.fY - Desc.fSizeY * 0.5f), long(Desc.fX + Desc.fSizeX * 0.5f), long(Desc.fY + Desc.fSizeY * 0.5f) };

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Create_HpBar(TEXT("Layer_Enemy_HpBar"))))
		return E_FAIL;

	m_pUI_Manager = CBattle_UI_Manager::GetInstance();
	m_pUI_Manager->Set_Hp(this);
	
	return S_OK;
}

void CBattle_Enemy_Hp_BG::Priority_Update(_float fTimeDelta)
{
}

void CBattle_Enemy_Hp_BG::Update(_float fTimeDelta)
{
	if (m_bActive)
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

		m_pHpBar->Set_Parent_WorldPos(m_pTransformCom->Get_State(STATE::POSITION));


		m_fCurrentHp = static_cast<_float>(m_pEnemy->Get_CurrentHp());
		m_pHpBar->Set_CurrentHp(m_fCurrentHp);

		_itow_s(static_cast<_int>(m_fMaxHp), m_szMaxHp, MAX_PATH, 10);
		_itow_s(static_cast<_int>(m_fCurrentHp), m_szCurrentHp, MAX_PATH, 10);

		m_fHpRatio = m_fCurrentHp / m_fMaxHp;

		if (m_fHpRatio <= 0.f)
			m_fHpRatio = 0.f;

		if (m_bDissolve)
			m_fTime += fTimeDelta;

		if (m_fTime >= 0.5f)
		{
			m_pUI_Manager->Update_Enemy_HP();
			m_bActive = false;
		}
	}
}

void CBattle_Enemy_Hp_BG::Late_Update(_float fTimeDelta)
{
	if (m_bActive)
	{
		m_pGameInstance->Add_RenderGroup(RENDER::BATTLEUI, this);
	}
}

HRESULT CBattle_Enemy_Hp_BG::Render()
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

		m_pHpBar->Render();


		_float InfoState = (m_pGameInstance->FontSizeX(TEXT("14"), m_pEnemy->Get_Digimon_Name().c_str()) - 12.f) * 0.5f + 10.f;
		m_pGameInstance->Render_Text(TEXT("14"), m_pEnemy->Get_Digimon_Name().c_str(), _float2(m_fX - InfoState, m_fY - 30.f));

		m_pGameInstance->Render_Text(TEXT("12"), m_szMaxHp, _float2(m_fX, m_fY - 2.5f));

		m_pGameInstance->Render_Text(TEXT("12"), TEXT("/"), _float2(m_fX - 10.f, m_fY - 2.5f));

		_float CurrentHp = (m_pGameInstance->FontSizeX(TEXT("12"), m_szCurrentHp) - 11.f);
		m_pGameInstance->Render_Text(TEXT("12"), m_szCurrentHp, _float2(m_fX - CurrentHp - 20.f, m_fY - 2.5f));
	}


	return S_OK;
}
void CBattle_Enemy_Hp_BG::Set_Move(_float fX, _float fY)
{
	m_fX = fX;
	m_fY = fY;
}

void CBattle_Enemy_Hp_BG::Set_Enemy(CContainerObject* Enemy)
{
	m_pEnemy = Enemy;
	m_fMaxHp = static_cast<_float>(m_pEnemy->Get_Hp());
	m_pHpBar->Set_MaxHp(m_fMaxHp);
}

void CBattle_Enemy_Hp_BG::Set_Dissolve(_bool Dissolve)
{
	m_bDissolve = Dissolve;
	m_pHpBar->Set_Dissolve(Dissolve);
}

HRESULT CBattle_Enemy_Hp_BG::Ready_Components()
{
	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Battle_HpBG*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Battle_Hp_BG"),
		TEXT("Com_Battle_HpBG"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_Battle_Dissolve*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Noise"),
		TEXT("Com_Battle_Dissolve"), reinterpret_cast<CComponent**>(&m_pDissolveTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_Battle_Enemy_Hp"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CBattle_Enemy_Hp_BG::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture1", 0)))
		return E_FAIL;
	if (FAILED(m_pDissolveTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DessolveTexture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_State("DissolveTime", m_fTime)))
		return E_FAIL;



	return S_OK;
}

HRESULT CBattle_Enemy_Hp_BG::Create_HpBar(const _wstring& strLayerTag)
{
	m_pHpBar = static_cast<CBattle_Enemy_HpBar*>(m_pGameInstance->Add_GameObject_ToLayer_ToCreate(
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Battle_Enemy_Hp"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag));
	if (m_pHpBar == nullptr)
		return E_FAIL;

	m_pHpBar->Set_Move(0.f, 10.f);

	return S_OK;
}

CBattle_Enemy_Hp_BG* CBattle_Enemy_Hp_BG::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBattle_Enemy_Hp_BG* pInstance = new CBattle_Enemy_Hp_BG(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBattle_Turn ");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CBattle_Enemy_Hp_BG::Clone(void* pArg)
{
	CBattle_Enemy_Hp_BG* pInstance = new CBattle_Enemy_Hp_BG(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created :CBattle_Turn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBattle_Enemy_Hp_BG::Free()
{
	__super::Free();

	Safe_Release(m_pDissolveTextureCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
}

