#include "Digivice.h"
#include "GameInstance.h"
#include "Digivice_Slot.h"
#include "Digimon_Manager.h"
#include "Battle_Manager.h"
#include "Digivice_Mask.h"
#include "Digivice_Info.h"
#include "Digivice_Skill.h"
#include "Digivice_Target.h"
#include "Digivice_Hp.h"
#include "Digivice_Sp.h"
#include "Digivice_Exp.h"

CDigivice::CDigivice(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CDigivice::CDigivice(const CDigivice& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CDigivice::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDigivice::Initialize(void* pArg)
{
	CUIObject::UIOBJECT_DESC	Desc{};

	Desc.fX = 700.f;
	Desc.fY = 350.f;
	Desc.fSizeX = 900.f;
	Desc.fSizeY = 600.f;

	m_iDigivice_Battle_Slot_Number = 8;
	m_iDigivice_Battle_Skill_Number = 3;

	m_pDigimon_Manager = CDigimon_Manager::GetInstance();
	m_pDigimon_Manager->Digivice(this);

	m_pBattle_Manager = CBattle_Manager::GetInstance();
	m_pBattle_Manager->Set_Digivice(this);

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Create_Slot(L"Layout_Digivice_Slot")))
		return E_FAIL;

	if (FAILED(Create_Skill(L"Layer_Digivice_Skill")))
		return E_FAIL;

	if (FAILED(Create_Target(L"Layer_Digivice_Target")))
		return E_FAIL;

	if (FAILED(Craete_State(L"Layer_Digivice_State")))
		return E_FAIL;

	m_Digimon_ID.resize(8, -1);

	Acquire_Digimon(1);
	Acquire_Digimon(9);
	Acquire_Digimon(7);
	m_strDigimon_Stage[0] = L"유아기";
	m_strDigimon_Stage[1] = L"성장기";
	m_strDigimon_Stage[2] = L"성숙기";
	m_strDigimon_Stage[3] = L"완전체";
	m_strDigimon_Stage[4] = L"궁극체";
	m_strDigimon_Attribute[0] = L"데이터";
	m_strDigimon_Attribute[1] = L"바이러스";
	m_strDigimon_Attribute[2] = L"백신";
	return S_OK;
}

void CDigivice::Priority_Update(_float fTimeDelta)
{
}

void CDigivice::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Down(DIK_V))
	{
		m_pSelectSlot = m_pBattle_Slot[0];
		for (int j = 0; j < m_pSelectSlot->Get_DigimonInfo().SkillCount; ++j)
		{
			m_pBattle_Skill[j]->Set_Digimon_SkillSet(m_pSelectSlot->Get_DigimonInfo().DigimonId, j);
		}
		Set_State(m_pSelectSlot->Get_DigimonInfo());
		Set_Active(!m_bActive);
	}

	m_pTransformCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
	for (_uint i = 0; i < m_iDigivice_Battle_Slot_Number; ++i)
	{
		m_pBattle_Slot[i]->Set_Parent_WorldPos(m_pTransformCom->Get_State(STATE::POSITION));
		m_pBattle_Mask[i]->Set_Parent_WorldPos(m_pTransformCom->Get_State(STATE::POSITION));
	}

	m_pDigivice_Info->Set_Parent_WorldPos(m_pTransformCom->Get_State(STATE::POSITION));

	if (m_bActive)
	{
		OnClick();
		OnHover();
	}

	if (m_bActive)
	{
		_itow_s(m_pSelectSlot->Get_DigimonInfo().Hp, m_szDigimonHp, MAX_PATH, 10);
		_itow_s(m_pSelectSlot->Get_DigimonInfo().CurrentHp, m_szCurrentHp, MAX_PATH, 10);
		_itow_s(m_pSelectSlot->Get_DigimonInfo().Sp, m_szDigimonSp, MAX_PATH, 10);
		_itow_s(m_pSelectSlot->Get_DigimonInfo().CurrentSp, m_szCurrentSp, MAX_PATH, 10);
		_itow_s(m_pSelectSlot->Get_DigimonInfo().Damage, m_szDigimonDamage, MAX_PATH, 10);
		_itow_s(m_pSelectSlot->Get_DigimonInfo().AttackSpeed, m_szDigimonAttackSpeed, MAX_PATH, 10);
		_itow_s(m_pSelectSlot->Get_DigimonInfo().Exp, m_szDigimonMaxExp, MAX_PATH, 10);
		_itow_s(m_pSelectSlot->Get_DigimonInfo().CurrentExp, m_szDigimonCurrentExp, MAX_PATH, 10);
	}
}

void CDigivice::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::UI, this);
}

HRESULT CDigivice::Render()
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

		for (_uint i = 0; i < m_iDigivice_Battle_Slot_Number; ++i)
		{
			m_pBattle_Slot[i]->Render();
			if (m_pBattle_Mask[i]->Get_HasDigimon())
				m_pBattle_Mask[i]->Render();
		}

		m_pDigivice_Info->Render();


		_float State = (m_pGameInstance->FontSizeX(TEXT("14"), TEXT("세대")) - 18.f) * 0.5f;
		_float Attribute = (m_pGameInstance->FontSizeX(TEXT("14"), TEXT("타입")) - 18.f) * 0.5f;
		_float MaxHp = (m_pGameInstance->FontSizeX(TEXT("14"), TEXT("최대 HP")) - 18.f) * 0.5f;
		_float MaxSp = (m_pGameInstance->FontSizeX(TEXT("14"), TEXT("최대 SP")) - 18.f) * 0.5f;
		_float Damage = (m_pGameInstance->FontSizeX(TEXT("14"), TEXT("공격력")) - 18.f) * 0.5f;
		_float AttackSpeed = (m_pGameInstance->FontSizeX(TEXT("14"), TEXT("공격 속도")) - 18.f) * 0.5f;

		_float InfoState = (m_pGameInstance->FontSizeX(TEXT("14"), m_strDigimon_Stage[ENUM_CLASS(m_pSelectSlot->Get_DigimonInfo().Stage)].c_str()) - 12.f) * 0.5f;
		_float InfoAttribute = (m_pGameInstance->FontSizeX(TEXT("14"), m_strDigimon_Attribute[ENUM_CLASS(m_pSelectSlot->Get_DigimonInfo().Attribute)].c_str()) - 12.f) * 0.5f;
		_float InfoMaxHp = (m_pGameInstance->FontSizeX(TEXT("14"), m_szDigimonHp) - 12.f) * 0.5f;
		_float InfoCurrentHp = m_pGameInstance->FontSizeX(TEXT("14"),m_szCurrentHp) - 12.f;
		_float InfoMaxSp = (m_pGameInstance->FontSizeX(TEXT("14"), m_szDigimonSp) - 12.f) * 0.5f;
		_float InfoCurrentSp = m_pGameInstance->FontSizeX(TEXT("14"), m_szCurrentSp) - 12.f;
		_float InfoCurrentExp = m_pGameInstance->FontSizeX(TEXT("14"), m_szDigimonCurrentExp) - 12.f;
		_float InfoDamage = (m_pGameInstance->FontSizeX(TEXT("14"), m_szDigimonDamage) - 12.f) * 0.5f;
		_float InfoAttackSpeed = (m_pGameInstance->FontSizeX(TEXT("14"), m_szDigimonAttackSpeed) - 12.f) * 0.5f;

		m_pGameInstance->Render_Text(TEXT("18"), TEXT("디지바이스"), _float2(620.f, 70.f), XMVectorSet(0.f, 0.f, 1.f, 1.f));
		m_pGameInstance->Render_Text(TEXT("17"), m_pSelectSlot->Get_DigimonInfo().DigimonName.c_str(), _float2(600.f, 120.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
		m_pGameInstance->Render_Text(TEXT("14"), TEXT("세대"), _float2(880.f - State, 170.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
		m_pGameInstance->Render_Text(TEXT("14"), TEXT("타입"), _float2(880.f - Attribute, 200.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
		m_pGameInstance->Render_Text(TEXT("14"), TEXT("최대 HP"), _float2(880.f - MaxHp, 230.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
		m_pGameInstance->Render_Text(TEXT("14"), TEXT("최대 SP"), _float2(880.f - MaxSp, 260.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
		m_pGameInstance->Render_Text(TEXT("14"), TEXT("공격력"), _float2(880.f - Damage, 290.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
		m_pGameInstance->Render_Text(TEXT("14"), TEXT("공격 속도"), _float2(880.f - AttackSpeed, 320.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));

		m_pGameInstance->Render_Text(TEXT("15"), m_pSelectSlot->Get_DigimonInfo().DigimonSkill1Info.SkillName.c_str(), _float2(637.f, 476.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
		m_pGameInstance->Render_Text(TEXT("15"), m_pSelectSlot->Get_DigimonInfo().DigimonSkill2Info.SkillName.c_str(), _float2(637.f, 536.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
		m_pGameInstance->Render_Text(TEXT("15"), m_pSelectSlot->Get_DigimonInfo().DigimonSkill3Info.SkillName.c_str(), _float2(637.f, 596.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));


		m_pGameInstance->Render_Text(TEXT("14"), m_strDigimon_Stage[ENUM_CLASS(m_pSelectSlot->Get_DigimonInfo().Stage)].c_str(), _float2(1050.f - InfoState, 170.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
		m_pGameInstance->Render_Text(TEXT("14"), m_strDigimon_Attribute[ENUM_CLASS(m_pSelectSlot->Get_DigimonInfo().Attribute)].c_str(), _float2(1050.f - InfoAttribute, 200.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
		m_pGameInstance->Render_Text(TEXT("14"), m_szDigimonHp, _float2(1050.f - InfoMaxHp, 230.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
		m_pGameInstance->Render_Text(TEXT("14"), m_szDigimonSp, _float2(1050.f - InfoMaxSp, 260.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
		m_pGameInstance->Render_Text(TEXT("14"), m_szDigimonDamage, _float2(1050.f - InfoDamage, 290.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
		m_pGameInstance->Render_Text(TEXT("14"), m_szDigimonAttackSpeed, _float2(1050.f - InfoAttackSpeed, 320.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));

		m_pGameInstance->Render_Text(TEXT("14"), TEXT("HP"), _float2(860.f, 380.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
		m_pGameInstance->Render_Text(TEXT("13"), m_szDigimonHp, _float2(1060.f, 400.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
		m_pGameInstance->Render_Text(TEXT("as"), TEXT("/"), _float2(1050.f, 405.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
		m_pGameInstance->Render_Text(TEXT("13"), m_szCurrentHp, _float2(1040.f - InfoCurrentHp, 400.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
		
		m_pGameInstance->Render_Text(TEXT("14"), TEXT("SP"), _float2(860.f, 450.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
		m_pGameInstance->Render_Text(TEXT("13"), m_szDigimonSp, _float2(1060.f, 470.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
		m_pGameInstance->Render_Text(TEXT("as"), TEXT("/"), _float2(1050.f, 475.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
		m_pGameInstance->Render_Text(TEXT("13"), m_szCurrentSp, _float2(1040 - InfoCurrentSp, 470.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
		
		m_pGameInstance->Render_Text(TEXT("14"), TEXT("EXP"), _float2(860.f, 520.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
		m_pGameInstance->Render_Text(TEXT("13"), m_szDigimonMaxExp, _float2(1060.f, 540.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
		m_pGameInstance->Render_Text(TEXT("as"), TEXT("/"), _float2(1050.f, 545.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
		m_pGameInstance->Render_Text(TEXT("13"), m_szDigimonCurrentExp, _float2(1040 - InfoCurrentExp, 540.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));


		m_pDigivice_Target->Render();

		m_pDigivice_Hp->Render();
		m_pDigivice_Sp->Render();
		m_pDigivice_Exp->Render();

		for (_uint i = 0; i < m_iDigivice_Battle_Skill_Number; ++i)
		{
			m_pBattle_Skill[i]->Render();
		}

	}
	return S_OK;
}

void CDigivice::Acquire_Digimon(_int ID)
{
	for (_uint i = 0; i < m_iDigivice_Battle_Slot_Number; ++i)
	{
		if (false == m_pBattle_Mask[i]->Get_HasDigimon())
		{
			m_pBattle_Mask[i]->Set_Digimon_ID(ID);
			m_pBattle_Mask[i]->Set_HasDigimon(true);
			m_pBattle_Slot[i]->Set_Info(Set_Info(ID));
			m_pBattle_Slot[i]->Set_HasDigimon(true);
			m_pDigimon_Manager->Set_Digivice_Slot(i, true, Info);
			m_Digimon_ID[i] = ID;
			for (int j = 0; j < m_pBattle_Slot[i]->Get_DigimonInfo().SkillCount; ++j)
			{
				m_pBattle_Skill[j]->Set_Digimon_SkillSet(ID, j);
			}
			return;
		}
	}
}

void CDigivice::Release_Digimon(_int ID)
{
	if (true == m_pBattle_Mask[ID]->Get_HasDigimon())
	{
		m_pBattle_Mask[ID]->Set_Digimon_ID(-1);
		m_pBattle_Mask[ID]->Set_HasDigimon(false);
		m_pBattle_Slot[ID]->Set_HasDigimon(false);
		m_pDigimon_Manager->Set_Digivice_Slot(ID, false, nullptr);

	}
	else
		return;
}

void CDigivice::OnClick()
{
	POINT pPt = m_pGameInstance->Get_Mouse();

	pPt.x -= static_cast<_long>(m_fWinSizeX * 0.5);
	pPt.y -= static_cast<_long>(m_fWinSizeY * 0.5);

	for (_uint i = 0; i < m_iDigivice_Battle_Slot_Number; ++i)
	{
		if (m_pGameInstance->Mouse_Down(MOUSEKEYSTATE::LBUTTON))
		{
			if (PtInRect(m_pBattle_Slot[i]->Get_Pos(), pPt))
			{
				if (m_pBattle_Slot[i]->Get_HasDigimon())
				{
					m_pSelectSlot = m_pBattle_Slot[i];
					for (int j = 0; j < m_pSelectSlot->Get_DigimonInfo().SkillCount; ++j)
					{
						m_pBattle_Skill[j]->Set_Digimon_SkillSet(m_pSelectSlot->Get_DigimonInfo().DigimonId, j);
					}
				}
			}
		}
	}
}

void CDigivice::OnHover()
{
	POINT pPt = m_pGameInstance->Get_Mouse();

	pPt.x -= static_cast<_long>(m_fWinSizeX * 0.5);
	pPt.y -= static_cast<_long>(m_fWinSizeY * 0.5);

	for (_uint i = 0; i < m_iDigivice_Battle_Skill_Number; ++i)
	{
		if (PtInRect(m_pBattle_Skill[i]->Get_Pos(), pPt))
		{
			m_pBattle_Skill[i]->Set_Digimon_Skill_Info_Pos(static_cast<_float>(pPt.x), static_cast<_float>(pPt.y));
		}
		else
		{
			m_pBattle_Skill[i]->Set_Hover();
		}
	}
}

void CDigivice::Update_Digimopn()
{
	m_pBattle_Manager->Current_Digimon(&m_pBattle_Slot[0]->Get_DigimonInfo(), &m_pBattle_Slot[1]->Get_DigimonInfo(), &m_pBattle_Slot[2]->Get_DigimonInfo());
}

DIGIMON_INFO* CDigivice::Set_Info(_int ID)
{
	Info = m_pDigimon_Manager->Search_Digimon(ID);

	Info->Hp += m_pGameInstance->intRandom(500, 1000);
	Info->CurrentHp = Info->Hp;
	Info->Sp += m_pGameInstance->intRandom(250, 500);
	Info->CurrentSp = Info->Sp;
	Info->Damage += m_pGameInstance->intRandom(500, 1000);
	Info->AttackSpeed += m_pGameInstance->intRandom(1, 20);
	Info->ExpReward += m_pGameInstance->intRandom(500, 1000);
	Info->CurrentExp = 0;
	//pInfo->AttackSpeed += 40;

	if (Info->Stage == DIGIMON_STAGE::MEGA)
		Info->Lv += 5;
	else if (Info->Stage == DIGIMON_STAGE::ULTIMATE)
		Info->Lv += m_pGameInstance->intRandom(1, 44);
	else if (Info->Stage == DIGIMON_STAGE::CHAMPION)
		Info->Lv += m_pGameInstance->intRandom(1, 29);
	else
		Info->Lv += m_pGameInstance->intRandom(1, 14);

	m_pDigimon_Manager->Set_Current_Digimon_Info(Info);

	return Info;
}

void CDigivice::Set_State(DIGIMON_INFO ID)
{
	m_pDigivice_Hp->Set_MaxHp(ID.Hp);
	m_pDigivice_Sp->Set_MaxSp(ID.Sp);
	m_pDigivice_Exp->Set_MaxExp(ID.Exp);
}

HRESULT CDigivice::Ready_Components()
{
	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Digivice"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_Inventory"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CDigivice::Bind_ShaderResources()
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

HRESULT CDigivice::Create_Slot(const _wstring& strLayerTag)
{
	for (_uint i = 0; i < m_iDigivice_Battle_Slot_Number; ++i)
	{
		m_pDigivice_Slot = static_cast<CDigivice_Slot*>(m_pGameInstance->Add_GameObject_ToLayer_ToCreate(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Digivice_Slot"),
			ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag));
		if (m_pDigivice_Slot == nullptr)
			return E_FAIL;

		m_pDigivice_Mask = static_cast<CDigivice_Mask*>(m_pGameInstance->Add_GameObject_ToLayer_ToCreate(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Digivice_Mask"),
			ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Mask")));
		if (m_pDigivice_Mask == nullptr)
			return E_FAIL;

		m_pBattle_Slot.push_back(m_pDigivice_Slot);
		m_pBattle_Mask.push_back(m_pDigivice_Mask);
	}

	m_pDigivice_Info = static_cast<CDigivice_Info*>(m_pGameInstance->Add_GameObject_ToLayer_ToCreate(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Digivice_Info"),
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Digivice_Info")));


	for (_uint i = 0; i < m_iDigivice_Battle_Slot_Number; ++i)
	{
		_float row = static_cast<_float>(i);

		_float startX = -300.f;
		_float startY = -170.f + row * 55;

		_float maskstartX = -50.f;

		m_pBattle_Slot[i]->Set_Move(startX, startY);
	}

	for (_uint i = 3; i < m_iDigivice_Battle_Slot_Number; ++i)
	{
		_float row = static_cast<_float>(i - 3);

		_float startX = -300.f;
		_float startY = 30.f + row * 55;

		_float maskstartX = -50.f;

		m_pBattle_Slot[i]->Set_Move(startX, startY);
	}

	for (_uint i = 0; i < m_iDigivice_Battle_Slot_Number; ++i)
	{
		_float row = static_cast<_float>(i);

		_float startX = 100.f;
		_float startY = 10.f + row * 110;

		_float maskstartX = -50.f;

		m_pBattle_Mask[i]->Set_Move(maskstartX, startY);

	}

	for (_uint i = 3; i < m_iDigivice_Battle_Slot_Number; ++i)
	{
		_float row = static_cast<_float>(i - 3);

		_float startX = 100.f;
		_float startY = 411.f + row * 110;

		_float maskstartX = -50.f;

		m_pBattle_Mask[i]->Set_Move(maskstartX, startY);
	}

	m_pDigivice_Info->Set_Move(285, 30);

	return S_OK;
}

HRESULT CDigivice::Create_Skill(const _wstring& strLayerTag)
{
	for (_uint i = 0; i < m_iDigivice_Battle_Skill_Number; ++i)
	{
		m_pDigivice_Skill = static_cast<CDigivice_Skill*>(m_pGameInstance->Add_GameObject_ToLayer_ToCreate(
			ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Digivice_Skill"), ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag));
		if (m_pDigivice_Skill == nullptr)
			return E_FAIL;

		m_pBattle_Skill.push_back(m_pDigivice_Skill);
	}

	for (_uint i = 0; i < m_iDigivice_Battle_Skill_Number; ++i)
	{

		_float row = static_cast<_float>(i);

		_float startX = -50.f;
		_float startY = 130.f + row * 60;


		m_pBattle_Skill[i]->Set_Move(startX, startY);

	}

	return S_OK;
}

HRESULT CDigivice::Create_Target(const _wstring& strLayerTag)
{
	m_pDigivice_Target = static_cast<CDigivice_Target*>(m_pGameInstance->Add_GameObject_ToLayer_ToCreate(
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Digivice_Target"), ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag));
	if (m_pDigivice_Target == nullptr)
		return E_FAIL;

	m_pDigivice_Target->Set_Move(45.f, -50.f);

	return S_OK;
}

HRESULT CDigivice::Craete_State(const _wstring& strLayerTag)
{
	m_pDigivice_Hp = static_cast<CDigivice_Hp*>(m_pGameInstance->Add_GameObject_ToLayer_ToCreate(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Digivice_Hp"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag));
	if (m_pDigivice_Hp == nullptr)
		return E_FAIL;

	m_pDigivice_Hp->Set_Move(985.f, 430.f);

	m_pDigivice_Sp = static_cast<CDigivice_Sp*>(m_pGameInstance->Add_GameObject_ToLayer_ToCreate(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Digivice_Sp"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag));
	if (m_pDigivice_Sp == nullptr)
		return E_FAIL;

	m_pDigivice_Sp->Set_Move(985.f, 500.f);

	m_pDigivice_Exp = static_cast<CDigivice_Exp*>(m_pGameInstance->Add_GameObject_ToLayer_ToCreate(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Digivice_Exp"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag));
	if (m_pDigivice_Exp == nullptr)
		return E_FAIL;

	m_pDigivice_Exp->Set_Move(985.f, 570.f);

	return S_OK;
}


CDigivice* CDigivice::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDigivice* pInstance = new CDigivice(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDigivice");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDigivice::Clone(void* pArg)
{
	CDigivice* pInstance = new CDigivice(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDigivice");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDigivice::Free()
{
	__super::Free();

	m_pBattle_Slot.clear();
	m_pBattle_Mask.clear();
	m_pBattle_Skill.clear();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}