#include "Digimon_Manager.h"
#include "PartyUHD.h"
#include "Digivice.h"
#include "Digimon_Storage.h"


IMPLEMENT_SINGLETON(CDigimon_Manager)

CDigimon_Manager::CDigimon_Manager()
{
}

HRESULT CDigimon_Manager::Initialize()
{
	m_iDigivice_Number = 8;

	m_iDigimon_Storage_Slot_Number = 10;

	return S_OK;
}

void CDigimon_Manager::Swap_Digimon()
{

}

void CDigimon_Manager::Digimon_UHD()
{

}

void CDigimon_Manager::Acquire_Digimon()
{
	for (_uint i = 0; i < m_iDigivice_Number; ++i)
	{
		if (m_vDigivice_Battle_Slot[i] != nullptr)
		{
			m_vDigivice_Battle_Slot[i];
		}
	}
}

void CDigimon_Manager::PartyUHD(CPartyUHD* pPartyUHD)
{
	m_pPartyUHD = pPartyUHD;
}

void CDigimon_Manager::Digivice(CDigivice* pDigivice)
{
	m_pDigivice = pDigivice;
}

void CDigimon_Manager::Digimon_Storage(CDigimon_Storage* pDigimon_Storage)
{
	m_pDigimon_Storage = pDigimon_Storage;
}

void CDigimon_Manager::Free()
{
	__super::Free();

	/*Safe_Release(m_pPartyUHD);
	Safe_Release(m_pDigivice);
	Safe_Release(m_pDigimon_Storage);*/
}
