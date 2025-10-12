#include "Battle_UI_Manager.h"

IMPLEMENT_SINGLETON(CBattle_UI_Manager)

CBattle_UI_Manager::CBattle_UI_Manager()
{
}

HRESULT CBattle_UI_Manager::Initialize()
{

	return S_OK;
}

void CBattle_UI_Manager::Set_MyDigimon(DIGIMON_INFO* MyDigimon)
{
	m_MyDigimon_Info.push_back(MyDigimon);
}

void CBattle_UI_Manager::Set_Enemy_Digimon(DIGIMON_INFO* EnemyDigimon)
{
	m_EnemyDigimon_Info.push_back(EnemyDigimon);
}

void CBattle_UI_Manager::Free()
{
	__super::Free();
}
