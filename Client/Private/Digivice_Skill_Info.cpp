#include "Digivice_Skill_Info.h"
#include "GameInstance.h"
#include "Digimon_Manager.h"

CDigivice_Skill_Info::CDigivice_Skill_Info(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CDigivice_Skill_Info::CDigivice_Skill_Info(const CDigivice_Skill_Info& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CDigivice_Skill_Info::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDigivice_Skill_Info::Initialize(void* pArg)
{
	CUIObject::UIOBJECT_DESC	Desc{};

	Desc.fX = 100.f;
	Desc.fY = 100.f;
	Desc.fSizeX = 400.f;
	Desc.fSizeY = 300.f;

	m_pRect = { long(Desc.fX - Desc.fSizeX * 0.5f), long(Desc.fY - Desc.fSizeY * 0.5f), long(Desc.fX + Desc.fSizeX * 0.5f), long(Desc.fY + Desc.fSizeY * 0.5f) };

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pManager = CDigimon_Manager::GetInstance();

	m_strDigimon_Terget[0] = L"자신";
	m_strDigimon_Terget[1] = L"아군";
	m_strDigimon_Terget[2] = L"적군";
	m_strDigimon_Range[0] = L"단일";
	m_strDigimon_Range[1] = L"전체";

	return S_OK;
}

void CDigivice_Skill_Info::Priority_Update(_float fTimeDelta)
{
	m_bHover = false;
}

void CDigivice_Skill_Info::Update(_float fTimeDelta)
{
	_itow_s(m_Info.Sp, m_szSkillSp, MAX_PATH, 10);
	_itow_s(m_Info.HitCount, m_szSkillCount, MAX_PATH, 10);
}

void CDigivice_Skill_Info::Late_Update(_float fTimeDelta)
{
	_vector vWorldPos = XMVectorSet(m_fX + m_fParent_WorldPos.x, -(m_fY)+m_fParent_WorldPos.y, 0.f, 1.f);

	m_pTransformCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(STATE::POSITION, vWorldPos);

	_float4 Pos;
	XMStoreFloat4(&Pos, vWorldPos);

	m_pRect = {
		long(Pos.x - m_fSizeX * 0.5f),
		long(-Pos.y - m_fSizeY * 0.5f),
		long(Pos.x + m_fSizeX * 0.5f),
		long(-Pos.y + m_fSizeY * 0.5f)
	};

	m_pGameInstance->Add_RenderGroup(RENDER::BATTLEUI, this);

}

HRESULT CDigivice_Skill_Info::Render()
{
	if (m_bHover)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Bind_Resources()))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;

		_float Sp = (m_pGameInstance->FontSizeX(TEXT("14"), TEXT("소모 SP : ")));
		_float Count= (m_pGameInstance->FontSizeX(TEXT("14"), TEXT("공격 횟수 : ")));
		_float Damage = (m_pGameInstance->FontSizeX(TEXT("14"), TEXT("데미지 : ")));
		_float fTarget = (m_pGameInstance->FontSizeX(TEXT("14"), TEXT("적용 대상 : ")));
		_float fRange = (m_pGameInstance->FontSizeX(TEXT("14"), TEXT("적용 범위 : ")));

		m_pGameInstance->Render_Text(TEXT("15"), m_Info.SkillName.c_str(), _float2(m_fX + 550, m_fY + 240));
		m_pGameInstance->Render_Text(TEXT("14"), TEXT("소모 SP : "), _float2(m_fX + 450, m_fY + 300));
		m_pGameInstance->Render_Text(TEXT("14"), m_szSkillSp, _float2(m_fX + 450 + Sp, m_fY + 300));
		m_pGameInstance->Render_Text(TEXT("14"), TEXT("공격 횟수 : "), _float2(m_fX + 450, m_fY + 330));
		m_pGameInstance->Render_Text(TEXT("14"), m_szSkillCount, _float2(m_fX + 450 + Count, m_fY + 330));
		m_pGameInstance->Render_Text(TEXT("14"), TEXT("데미지 : "), _float2(m_fX + 450, m_fY + 360));
		m_pGameInstance->Render_Text(TEXT("14"), m_Info.DamageInfo.c_str(), _float2(m_fX + 450 + Damage, m_fY + 360));
		m_pGameInstance->Render_Text(TEXT("14"), TEXT("적용 대상 : "), _float2(m_fX + 450, m_fY + 390));
		m_pGameInstance->Render_Text(TEXT("14"), m_strDigimon_Terget[ENUM_CLASS(m_Info.Target)].c_str(), _float2(m_fX + 450 + fTarget, m_fY + 390));
		m_pGameInstance->Render_Text(TEXT("14"), TEXT("적용 범위 : "), _float2(m_fX + 450, m_fY + 420));
		m_pGameInstance->Render_Text(TEXT("14"), m_strDigimon_Range[ENUM_CLASS(m_Info.Range)].c_str(), _float2(m_fX + 450 + fRange, m_fY + 420));
		m_pGameInstance->Render_Text(TEXT("14"), m_Info.SkillInfo.c_str(), _float2(m_fX + 450, m_fY + 450));

	}
	return S_OK;
}

void CDigivice_Skill_Info::Set_Info(SKILL_INFO* pInfo)
{
	m_Info = *pInfo;
}

void CDigivice_Skill_Info::Set_Parent_WorldPos(_vector fParent_World)
{
	XMStoreFloat4(&m_fParent_WorldPos, fParent_World);
}

void CDigivice_Skill_Info::Set_Move(_float fX, _float fY)
{
	if (fX >= 350)
	{
		m_fX = fX - m_fSizeX * 0.5f;
		m_fY = fY - m_fSizeY * 0.5f;
	}
	else
	{
		m_fX = fX + m_fSizeX * 0.5f;
		m_fY = fY - m_fSizeY * 0.5f;
	}
}

void CDigivice_Skill_Info::Set_Digimon_Skill_Info(_int ID, _int Count)
{
	switch (Count)
	{
	case 0:
		m_Info = m_pManager->Search_Digimon(ID)->DigimonSkill1Info;
		break;

	case 1:
		m_Info = m_pManager->Search_Digimon(ID)->DigimonSkill2Info;
			break;

	case 2:
		m_Info = m_pManager->Search_Digimon(ID)->DigimonSkill3Info;
			break;
	}
}

HRESULT CDigivice_Skill_Info::Ready_Components()
{
	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Skill_Info"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_Inventory"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CDigivice_Skill_Info::Bind_ShaderResources()
{
	/*m_pShaderCom->Bind_Matrix("g_WorldMatrix", );*/
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	return S_OK;
}

CDigivice_Skill_Info* CDigivice_Skill_Info::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDigivice_Skill_Info* pInstance = new CDigivice_Skill_Info(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDigivice_Skill_Info");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDigivice_Skill_Info::Clone(void* pArg)
{
	CDigivice_Skill_Info* pInstance = new CDigivice_Skill_Info(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDigivice_Skill_Info");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDigivice_Skill_Info::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}