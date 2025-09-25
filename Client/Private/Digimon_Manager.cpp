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

	Model_Name();
	Digimon_Prototypr();
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

const _wstring& CDigimon_Manager::Get_Model_ID(_int ID)
{
	wstring null = L"";
	auto iter = Find_Model(ID);

	if (nullptr == iter)
		return null;

	return *iter;
}

const _wstring& CDigimon_Manager::Get_Prototype_ID(_int ID)
{
	wstring null = L"";
	auto iter = Find_Prototype(ID);

	if (nullptr == iter)
		return null;

	return *iter;
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

void CDigimon_Manager::Model_Name()
{
	m_Digimon_Model.emplace(0,TEXT("Prototype_Component_Model_Angewomon"));
	m_Digimon_Model.emplace(1,TEXT("Prototype_Component_Model_Blackwargreymon"));
	m_Digimon_Model.emplace(2,TEXT("Prototype_Component_Model_Devilmon"));
	m_Digimon_Model.emplace(3,TEXT("Prototype_Component_Model_Ladydevimon"));
	m_Digimon_Model.emplace(4,TEXT("Prototype_Component_Model_Leomon"));
	m_Digimon_Model.emplace(5,TEXT("Prototype_Component_Model_Metalgarumon"));
	m_Digimon_Model.emplace(6,TEXT("Prototype_Component_Model_Metalgreymon"));
	m_Digimon_Model.emplace(7,TEXT("Prototype_Component_Model_Omegamon"));
	m_Digimon_Model.emplace(8,TEXT("Prototype_Component_Model_Wargreymon"));
}

void CDigimon_Manager::Digimon_Prototypr()
{
	m_Digimon_Prototype.emplace(0, TEXT("Prototype_GameObject_Angewomon"));
	m_Digimon_Prototype.emplace(1, TEXT("Prototype_GameObject_Blackwargreymon"));
	m_Digimon_Prototype.emplace(2, TEXT("Prototype_GameObject_Devilmon"));
	m_Digimon_Prototype.emplace(3, TEXT("Prototype_GameObject_Ladydevimon"));
	m_Digimon_Prototype.emplace(4, TEXT("Prototype_GameObject_Leomon"));
	m_Digimon_Prototype.emplace(5, TEXT("Prototype_GameObject_Metalgarumon"));
	m_Digimon_Prototype.emplace(6, TEXT("Prototype_GameObject_Metalgreymon"));
	m_Digimon_Prototype.emplace(7, TEXT("Prototype_GameObject_Omegamon"));
	m_Digimon_Prototype.emplace(8, TEXT("Prototype_GameObject_Wargreymon"));
}

DIGIMON_INFO* CDigimon_Manager::Find_Digimon(_int Digimon_ID)
{
	auto iter = m_Digimon_Info.find(Digimon_ID);

	if (iter == m_Digimon_Info.end())
		return nullptr;

	return &iter->second;
}

const _wstring* CDigimon_Manager::Find_Model(_int Digimon_ID)
{
	auto iter = m_Digimon_Model.find(Digimon_ID);

	if (iter == m_Digimon_Model.end())
		return nullptr;

	return &iter->second;
}

const _wstring* CDigimon_Manager::Find_Prototype(_int Digimon_ID)
{
	auto iter = m_Digimon_Prototype.find(Digimon_ID);

	if (iter == m_Digimon_Prototype.end())
		return nullptr;

	return &iter->second;
}

void CDigimon_Manager::Free()
{
	__super::Free();

	//for (auto& Pair : m_Digimon_Info)
	//	Safe_Release(Pair.second);

	Safe_Release(m_pPlayer);

	m_vDigivice_Battle_Slot.clear();
}
