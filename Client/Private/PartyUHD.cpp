#include "PartyUHD.h"

#include "UHD_Slot.h"

#include "GameInstance.h"
#include "Digimon_Manager.h"

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

	if (FAILED(Create_Slot(TEXT("Layer_UHDSlot"))))
		return E_FAIL;

    return S_OK;
}

void CPartyUHD::Priority_Update(_float fTimeDelta)
{
}

void CPartyUHD::Update(_float fTimeDelta)
{

}

void CPartyUHD::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::UI, this);
}

HRESULT CPartyUHD::Render()
{
	for (_uint i = 0; i < m_iSlotCount; ++i)
	{
		//if (m_vSlots[i]->Get_Digimon())
		//{
			m_vSlots[i]->Render();
		//}
	}
    return S_OK;
}

void CPartyUHD::Set_Digimon_ID(_uint Slot, _uint ID)
{
	m_vSlots[Slot]->Set_Digimon_ID(ID);
}

HRESULT CPartyUHD::Create_Slot(const _wstring& strLayerTag)
{
	for (_uint i = 0; i < m_iSlotCount; ++i)
	{
		m_pUHD_Slot = static_cast<CUHD_Slot*>(m_pGameInstance->Add_GameObject_ToLayer_ToCreate(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UHDSlot"),
			ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag));

		m_vSlots.push_back(m_pUHD_Slot);
	}

	for (_uint i = 0; i < m_iSlotCount; ++i)
	{
		_float row = i % 3;

		_float StartX = 70.f;
		_float StartY = 60.f + row * (90 + 10);

		m_vSlots[i]->Set_Move(StartX, StartY);
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
}

