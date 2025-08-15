#include "Digimon_Manager.h"

IMPLEMENT_SINGLETON(CDigimon_Manager)

CDigimon_Manager::CDigimon_Manager()
{
}

HRESULT CDigimon_Manager::Initialize()
{
    m_iDigivice_Battle_Slot_Number = 3;
    m_iDigivice_Serve_Slot_Number = 5;

    m_iDigimon_Storage_Slot_Number = 10;

    return S_OK;
}

void CDigimon_Manager::Swap_Digimon()
{

}

void CDigimon_Manager::Digimon_UHD()
{
}

void CDigimon_Manager::Free()
{
}
