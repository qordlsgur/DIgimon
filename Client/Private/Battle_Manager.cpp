#include "Battle_Manager.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CBattle_Manager);

CBattle_Manager::CBattle_Manager()
{
}

HRESULT CBattle_Manager::Initialize()
{
    m_pGameInstance = CGameInstance::GetInstance();

    return S_OK;
}

void CBattle_Manager::Battle_System()
{
    if (m_pGameInstance->Key_Down(DIK_Q))
    {

    }

    if (m_pGameInstance->Key_Down(DIK_W))
    {

    }

    if (m_pGameInstance->Key_Down(DIK_E))
    {

    }
}

void CBattle_Manager::Digimon1_Skill()
{
    if (m_pGameInstance->Key_Down(DIK_A))
    {

    }

    if (m_pGameInstance->Key_Down(DIK_S))
    {

    }

    if (m_pGameInstance->Key_Down(DIK_D))
    {

    }
}

void CBattle_Manager::Digimon2_Skill()
{
    if (m_pGameInstance->Key_Down(DIK_Z))
    {

    }

    if (m_pGameInstance->Key_Down(DIK_X))
    {

    }

    if (m_pGameInstance->Key_Down(DIK_C))
    {

    }
}

void CBattle_Manager::Digimon3_Skill()
{
}

void CBattle_Manager::Free()
{
    __super::Free();

    m_pMyDigimon.clear();
    m_pEnemyDigimon.clear();

}
