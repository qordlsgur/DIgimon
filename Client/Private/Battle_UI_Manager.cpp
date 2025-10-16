#include "Battle_UI_Manager.h"
#include "GameInstance.h"
#include "ContainerObject.h"
#include "Battle_Timeline.h"
#include "Battle_Turn.h"
#include "Battle_Skill.h"

IMPLEMENT_SINGLETON(CBattle_UI_Manager)

CBattle_UI_Manager::CBattle_UI_Manager()
{
}

HRESULT CBattle_UI_Manager::Initialize()
{
	m_pGameInstance = CGameInstance::GetInstance();

	return S_OK;
}

HRESULT CBattle_UI_Manager::Create_TimeLine()
{
	m_pTurn = static_cast<CBattle_Turn*>(m_pGameInstance->Add_GameObject_ToLayer_ToCreate(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Battle_Turn"),
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Battle_Turn")));
	if (m_pTurn == nullptr)
		return E_FAIL;

	for (_int i = 0; i < 6; ++i)
	{
		m_pTimeLine = static_cast<CBattle_Timeline*>(m_pGameInstance->Add_GameObject_ToLayer_ToCreate(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Battle_Timeline")
			, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Battle_TimeLine")));
		if (m_pTimeLine == nullptr)
			return E_FAIL;

		m_pTimeLine_array.push_back(m_pTimeLine);
	}

	m_fTurn_Panel[0] = _float2(1208.f, 200.f);
	m_fTurn_Panel[1] = _float2(1220.f, 268.f);
	m_fTurn_Panel[2] = _float2(1220.f, 328.f);
	m_fTurn_Panel[3] = _float2(1220.f, 388.f);
	m_fTurn_Panel[4] = _float2(1220.f, 448.f);
	m_fTurn_Panel[5] = _float2(1220.f, 508.f);

	m_pTimeLine_array[1]->Set_Move(m_fTurn_Panel[1]);
	m_pTimeLine_array[1]->Set_SizeDown();

	m_pTimeLine_array[2]->Set_Move(m_fTurn_Panel[2]);
	m_pTimeLine_array[2]->Set_SizeDown();

	m_pTimeLine_array[3]->Set_Move(m_fTurn_Panel[3]);
	m_pTimeLine_array[3]->Set_SizeDown();

	m_pTimeLine_array[4]->Set_Move(m_fTurn_Panel[4]);
	m_pTimeLine_array[4]->Set_SizeDown();

	m_pTimeLine_array[5]->Set_Move(m_fTurn_Panel[5]);
	m_pTimeLine_array[5]->Set_SizeDown();

	return S_OK;
}

void CBattle_UI_Manager::Set_Battle_Turn_Order(deque<CContainerObject*> Digimon_Turn_Order)
{
	m_pDigimon_Turn_Order = Digimon_Turn_Order;
}

void CBattle_UI_Manager::Set_MyDigimon(DIGIMON_INFO* MyDigimon)
{
	m_iDigimon++;
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

	m_pTurn->Set_Active(true);

	for (auto& Timeline : m_pTimeLines)
		Timeline->Set_Active(true);
}

void CBattle_UI_Manager::Turn_Start()
{
	m_pCurrent_TimeLine = m_pTimeLines.front();
	m_pTimeLines.pop_front();
	//m_pTimeLines.back()->Set_Active(true);
}

void CBattle_UI_Manager::Turn_End()
{
	m_pCurrent_TimeLine->Set_SizeDown();
	m_pCurrent_TimeLine->Set_Move(m_fTurn_Panel[m_pTimeLines.size()]);
	m_pCurrent_TimeLine->Set_Active(false);

	m_pTimeLines.push_back(m_pCurrent_TimeLine);

	m_pTimeLines[0]->Set_OffSet(m_fTurn_Panel[0]);
	m_pTimeLines[0]->Set_Lerp(true);

	for (size_t i = 1; i < m_pTimeLines.size(); ++i)
	{
		m_pTimeLines[i]->Set_SlowOffSet(m_fTurn_Panel[i]);
		m_pTimeLines[i]->Set_SlowLerp(true);
	}

}

HRESULT CBattle_UI_Manager::Create_Skill()
{
	for (size_t i = 0; i < m_MyDigimon_Info.size(); ++i)
	{
		for (_int i = 0; i < 3; ++i)
		{
			m_pDigimon_Skill = static_cast<CBattle_Skill*>(m_pGameInstance->Add_GameObject_ToLayer_ToCreate(
				ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Battle_Skill"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Battle_Digimon_Skill")));
			if (m_pDigimon_Skill == nullptr)
				return E_FAIL;

			m_pDigimon_Skills.push_back(m_pDigimon_Skill);
		}
	}
	return S_OK;
}

void CBattle_UI_Manager::Set_Skill()
{
	m_pDigimon_Skills[0]->Set_Move(300,650);
	m_pDigimon_Skills[0]->Set_Digimon_SkillSet(m_MyDigimon_Info[0]->DigimonId, 0);
	m_pDigimon_Skills[1]->Set_Move(365,650);
	m_pDigimon_Skills[1]->Set_Digimon_SkillSet(m_MyDigimon_Info[0]->DigimonId, 1);
	m_pDigimon_Skills[2]->Set_Move(430,650);
	m_pDigimon_Skills[2]->Set_Digimon_SkillSet(m_MyDigimon_Info[0]->DigimonId, 2);

	if (m_MyDigimon_Info.size() > 1)
	{
		m_pDigimon_Skills[3]->Set_Move(665, 650);
		m_pDigimon_Skills[3]->Set_Digimon_SkillSet(m_MyDigimon_Info[1]->DigimonId, 0);
		m_pDigimon_Skills[4]->Set_Move(730, 650);
		m_pDigimon_Skills[4]->Set_Digimon_SkillSet(m_MyDigimon_Info[1]->DigimonId, 1);
		m_pDigimon_Skills[5]->Set_Move(795, 650);
		m_pDigimon_Skills[5]->Set_Digimon_SkillSet(m_MyDigimon_Info[1]->DigimonId, 2);

	}

	if (m_MyDigimon_Info.size() > 2)
	{
		m_pDigimon_Skills[6]->Set_Move(1030, 650);
		m_pDigimon_Skills[6]->Set_Digimon_SkillSet(m_MyDigimon_Info[2]->DigimonId, 0);
		m_pDigimon_Skills[7]->Set_Move(1095, 650);
		m_pDigimon_Skills[7]->Set_Digimon_SkillSet(m_MyDigimon_Info[2]->DigimonId, 1);
		m_pDigimon_Skills[8]->Set_Move(1160, 650);
		m_pDigimon_Skills[8]->Set_Digimon_SkillSet(m_MyDigimon_Info[2]->DigimonId, 2);
	}
}

void CBattle_UI_Manager::Free()
{
	__super::Free();
}
