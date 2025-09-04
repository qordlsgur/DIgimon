#include "Inventory_Manager.h"

IMPLEMENT_SINGLETON(CInventory_Manager)

CInventory_Manager::CInventory_Manager()
{
}

HRESULT CInventory_Manager::Initialize()
{

    return S_OK;
}


void CInventory_Manager::Free()
{
    __super::Free();
}
