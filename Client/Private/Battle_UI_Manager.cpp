#include "Battle_UI_Manager.h"
#include "GameInstance.h"
#include "ContainerObject.h"
#include "Battle_Timeline.h"

IMPLEMENT_SINGLETON(CBattle_UI_Manager)

CBattle_UI_Manager::CBattle_UI_Manager()
{
}

HRESULT CBattle_UI_Manager::Initialize()
{
	m_pGameInstance = CGameInstance::GetInstance();

	Create_TimeLine();
	return S_OK;
}

HRESULT CBattle_UI_Manager::Create_TimeLine()
{
	m_pTimeLine = static_cast<CBattle_Timeline*>(m_pGameInstance->Add_GameObject_ToLayer_ToCreate(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Battle_Timeline")
		,ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Battle_TimeLine")));
	if (m_pTimeLine == nullptr)
		return E_FAIL;

	m_pTimeLine_array.push_back(m_pTimeLine);

	return S_OK;
}

void CBattle_UI_Manager::Set_Battle_Turn_Order(deque<CContainerObject*> Digimon_Turn_Order)
{
	m_pDigimon_Turn_Order = Digimon_Turn_Order;
}

void CBattle_UI_Manager::Set_MyDigimon(DIGIMON_INFO* MyDigimon)
{
	m_MyDigimon_Info.push_back(MyDigimon);
}

void CBattle_UI_Manager::Set_Enemy_Digimon(DIGIMON_INFO* EnemyDigimon)
{
	m_EnemyDigimon_Info.push_back(EnemyDigimon);
}

void CBattle_UI_Manager::Set_Timeline_Turn_Order()
{
	for (size_t i = 0; i < m_pDigimon_Turn_Order.size(); ++i)
	{
		if (false == m_pDigimon_Turn_Order[i]->Get_Monster())
			m_pTimeLine_array[i]->Set_Digimon(false, m_pDigimon_Turn_Order[i]->Get_ID());
		else
			m_pTimeLine_array[i]->Set_Digimon(true, m_pDigimon_Turn_Order[i]->Get_ID());
	}

	for (auto& Timeline : m_pTimeLine_array)
	{
		m_pTimeLines.push_back(Timeline);
	}
}

void CBattle_UI_Manager::Free()
{
	__super::Free();
}
