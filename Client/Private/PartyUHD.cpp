#include "PartyUHD.h"

#include "UHD_Slot.h"

#include "GameInstance.h"
#include "Digimon_Manager.h"
#include "Battle_Manager.h"

CPartyUHD::CPartyUHD(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUIObject{pDevice, pContext}
{
}

CPartyUHD::CPartyUHD(const CPartyUHD& Prototype)
    : CUIObject{Prototype}
{
}

HRESULT CPartyUHD::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CPartyUHD::Initialize(void* pArg)
{

	m_iSlotCount = 3;

	m_pDigimon_Manager = CDigimon_Manager::GetInstance();
	m_pDigimon_Manager->PartyUHD(this);

	m_pBattle_Manager = CBattle_Manager::GetInstance();

	if (FAILED(Create_Slot(TEXT("Layer_UHDSlot"))))
		return E_FAIL;

    return S_OK;
}

void CPartyUHD::Priority_Update(_float fTimeDelta)
{
}

void CPartyUHD::Update(_float fTimeDelta)
{
	m_bBattle = m_pBattle_Manager->Get_Battle();

	if (!m_bBattle)
		Set_NonBattle_Slot();
	else
		Set_Battle_Slot();

	for (_int i = 0; i < m_vSlots.size(); ++i)
	{
		m_vSlots[i]->Set_Digimon_Info(m_pDigimon_Manager->Get_Current_Digimon_Info(i));
	}
}
	
void CPartyUHD::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::UI, this);
}

HRESULT CPartyUHD::Render()
{
	if (!m_bBattle)
	{
		for (_uint i = 0; i < m_iSlotCount; ++i)
		{
			if (m_vSlots[i]->Get_Digimon())
			{
				m_vSlots[i]->Render();
				_itow_s(m_vSlots[i]->Get_Info().Lv, m_szDigimonLv, MAX_PATH, 10);
				wcscpy_s(m_szLv, MAX_PATH, L"LV ");
				wcscat_s(m_szLv, MAX_PATH, m_szDigimonLv);
				m_pGameInstance->Render_Rotation_Text(TEXT("13"), m_szLv, _float2(30, 90.f + 110.f * i), XMVectorSet(1.f, 1.f, 1.f, 1.f), 45.f);
			}
		}
	}
	else
	{
		for (_uint i = 0; i < m_iSlotCount; ++i)
		{
			if (m_vSlots[i]->Get_Digimon())
			{
				_float col = static_cast<_float>(i % 3);
				m_vSlots[i]->Render();
				_itow_s(m_vSlots[i]->Get_Info().Lv, m_szDigimonLv, MAX_PATH, 10);
				wcscpy_s(m_szLv, MAX_PATH, L"LV ");
				wcscat_s(m_szLv, MAX_PATH, m_szDigimonLv);
				_float Name = (m_pGameInstance->FontSizeX(TEXT("13"), m_vSlots[i]->Get_Info().DigimonName.c_str()) - 34.f) * 0.5f;
				m_pGameInstance->Render_Text(TEXT("13"), m_szLv, _float2(180 + col * (102 + 260), 575), XMVectorSet(1.f, 1.f, 1.f, 1.f));
				m_pGameInstance->Render_Text(TEXT("13"), m_vSlots[i]->Get_Info().DigimonName.c_str(), _float2((180 - Name) + col * (102 + 260), 690), XMVectorSet(1.f, 1.f, 1.f, 1.f));
			}
		}
	}

    return S_OK;
}

void CPartyUHD::Set_Digimon_ID(_uint Slot, _bool HasDigimon, DIGIMON_INFO* Info)
{
	m_vSlots[Slot]->Set_Digimon_Info(Info);
	m_vSlots[Slot]->Set_Digimon(HasDigimon);
}

void CPartyUHD::Set_NonBattle_Slot()
{
	for (_uint i = 0; i < m_iSlotCount; ++i)
	{
		_float row = static_cast<_float>(i % 3);

		_float StartX = 70.f;
		_float StartY = 60.f + row * (90 + 20);

		m_vSlots[i]->Set_Move(StartX, StartY);
	}
}

void CPartyUHD::Set_Battle_Slot()
{
	for (_uint i = 0; i < m_iSlotCount; ++i)
	{
		_float col = static_cast<_float>(i % 3);

		_float StartX = 200.f + col * (102 + 260);
		_float StartY = 650.f;

		m_vSlots[i]->Set_Move(StartX, StartY);
	}
}

HRESULT CPartyUHD::Create_Slot(const _wstring& strLayerTag)
{
	for (_uint i = 0; i < m_iSlotCount; ++i)
	{
		m_pUHD_Slot = static_cast<CUHD_Slot*>(m_pGameInstance->Add_GameObject_ToLayer_ToCreate(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UHDSlot"),
			ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag));

		m_vSlots.push_back(m_pUHD_Slot);
	}

    return S_OK;
}

CPartyUHD* CPartyUHD::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPartyUHD* pInstance = new CPartyUHD(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPartyUHD ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPartyUHD::Clone(void* pArg)
{
	CPartyUHD* pInstance = new CPartyUHD(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created :CPartyUHD");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPartyUHD::Free()
{
	__super::Free();

	//Safe_Release(m_pUHD_Slot);
}
