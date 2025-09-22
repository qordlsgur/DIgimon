#include "Digimon_Manager.h"
#include "PartyUHD.h"
#include "Digivice.h"


IMPLEMENT_SINGLETON(CDigimon_Manager)

CDigimon_Manager::CDigimon_Manager()
{
}

HRESULT CDigimon_Manager::Initialize()
{
	m_iDigivice_Number = 8;

	m_vDigivice_Battle_Slot.resize(m_iDigivice_Number);

	return S_OK;
}

void CDigimon_Manager::Swap_Digimon(_int Sour, _int Dest)
{
	if (m_vDigivice_Battle_Slot[Dest] == false)
	{

	}
}

void CDigimon_Manager::Digimon_UHD()
{

}

void CDigimon_Manager::Acquire_Digimon(_int ID)
{
	m_pDigivice->Acquire_Digimon(ID);
}

void CDigimon_Manager::Release_Digimon(_int ID)
{
	m_pDigivice->Release_Digimon(ID);
}

void CDigimon_Manager::Set_Digivice_Slot(_int SlotID, _bool Has, _int Digimon_ID)
{
	m_vDigivice_Battle_Slot[SlotID] = Has;
	if (SlotID <= 2)
	{
		m_pPartyUHD->Set_Digimon_ID(SlotID, Has,Digimon_ID);
	}
}

DIGIMON_INFO* CDigimon_Manager::Search_Digimon(_int ID)
{
	auto iter = Find_Digimon(ID);
		
	if (nullptr == iter)
		return nullptr;

	return iter;
}

void CDigimon_Manager::PartyUHD(CPartyUHD* pPartyUHD)
{
	m_pPartyUHD = pPartyUHD;
}

void CDigimon_Manager::Digivice(CDigivice* pDigivice)
{
	m_pDigivice = pDigivice;
}

void CDigimon_Manager::Player(CGameObject* pPlayer)
{
	m_pPlayer = pPlayer;
}

_vector CDigimon_Manager::PlayerPos()
{
	return m_pPlayer->Get_Transform();
}

HRESULT CDigimon_Manager::Digimon_Add(_int Digimon_ID, DIGIMON_INFO Info)
{
	if (nullptr != Find_Digimon(Digimon_ID))
		return E_FAIL;

	m_Digimon_Info.emplace(Digimon_ID, Info);

	return S_OK;
}

DIGIMON_INFO* CDigimon_Manager::Find_Digimon(_int Digimon_ID)
{
	auto iter = m_Digimon_Info.find(Digimon_ID);

	if (iter == m_Digimon_Info.end())
		return nullptr;

	return &iter->second;
}

void CDigimon_Manager::Free()
{
	__super::Free();

	//for (auto& Pair : m_Digimon_Info)
	//	Safe_Release(Pair.second);

	m_vDigivice_Battle_Slot.clear();
}
