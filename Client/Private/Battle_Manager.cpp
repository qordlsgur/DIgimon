#include "Battle_Manager.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CBattle_Manager);

CBattle_Manager::CBattle_Manager()
{
}

HRESULT CBattle_Manager::Initialize()
{
    return S_OK;
}

void CBattle_Manager::Free()
{
    __super::Free();
}
