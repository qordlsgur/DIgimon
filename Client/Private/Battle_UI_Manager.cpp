#include "Battle_UI_Manager.h"
#include "GameInstance.h"
#include "ContainerObject.h"
#include "Battle_Timeline.h"
#include "Battle_Turn.h"
#include "Battle_Skill.h"
#include "KeyBord.h"
#include "Battle_Enemy_Hp_BG.h"

IMPLEMENT_SINGLETON(CBattle_UI_Manager)

CBattle_UI_Manager::CBattle_UI_Manager()
{
}

HRESULT CBattle_UI_Manager::Initialize()
{
	m_pGameInstance = CGameInstance::GetInstance();

	return S_OK;
}

HRESULT CBattle_UI_Manager::Create_TimeLine(_int MyDigimonCount, _int EnemyDigimonCount)
{
	// 총 갯수를 받아서 이 갯수만큼 만들어 주고 위치를 다 잡아준다.
	m_iMyDigimonCount = MyDigimonCount;
	m_iEnemyDigimonCount = EnemyDigimonCount;
	_int m_iDigimonCount = m_iMyDigimonCount + m_iEnemyDigimonCount;
	m_pTurn = static_cast<CBattle_Turn*>(m_pGameInstance->Add_GameObject_ToLayer_ToCreate(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Battle_Turn"),
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Battle_Turn")));
	if (m_pTurn == nullptr)
		return E_FAIL;

	for (_int i = 0; i < m_iDigimonCount; ++i)
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

	for (_int i = 1; i < m_iDigimonCount; ++i)
	{
		m_pTimeLine_array[i]->Set_Move(m_fTurn_Panel[i]);
		m_pTimeLine_array[i]->Set_SizeDown();
	}

	return S_OK;
}

void CBattle_UI_Manager::Deleta_TimeLine()
{
	for (size_t i = 0; i < m_pTimeLine_array.size(); ++i)
	{
		Safe_Release(m_pTimeLine_array[i]);
	}
	m_pTimeLine_array.clear();
	for (size_t i = 0; i < m_pDigimon_Turn_Order.size(); ++i)
	{
		Safe_Release(m_pDigimon_Turn_Order[i]);
	}
	m_pDigimon_Turn_Order.clear();
	for (size_t i = 0; i < m_pEnemys.size(); ++i)
	{
		Safe_Release(m_pEnemys[i]);
	}
	m_pEnemys.clear();
	for (size_t i = 0; i < m_pPlayers.size(); ++i)
	{
		Safe_Release(m_pPlayers[i]);
	}

	m_pPlayers.clear();
}

void CBattle_UI_Manager::Set_Battle_Turn_Order(deque<CContainerObject*> Digimon_Turn_Order)
{
	if (m_bBattle)
	{

		// 받아온걸 그대로 저장해줌
		m_pDigimon_Turn_Order = Digimon_Turn_Order;
	}
}

void CBattle_UI_Manager::Set_Hpbar(CContainerObject* Enemy, _int Damage)
{
	//for (size_t i = 0; i < m_pEnemys.size(); ++i)
	//{
	//	if (Enemy == m_pEnemys[i])
	//		m_pEnemyHps[i]->Set_Damage(Damage);
	//}
}

void CBattle_UI_Manager::Set_MyDigimon(CContainerObject* MyDigimon)
{
	m_iDigimon++;
	m_pPlayers.push_back(MyDigimon);
}

void CBattle_UI_Manager::Set_Enemy_Digimon(CContainerObject* EnemyDigimon)
{
	m_pEnemys.push_back(EnemyDigimon);
}

void CBattle_UI_Manager::Set_Timeline_Turn_Order()
{
	if (m_bBattle)
	{

		// 이제 타임라인을 정해주는데 우선 디지몬이 아군인지 적군인지 아이디랑 혹시 모르니 정보까지 다 넘겨준다
		// 이러면 아이디 안받아도 될듯?
		for (size_t i = 0; i < m_pDigimon_Turn_Order.size(); ++i)
		{
			if (false == m_pDigimon_Turn_Order[i]->Get_Monster())
			{
				m_pTimeLine_array[i]->Set_Digimon(false, m_pDigimon_Turn_Order[i]->Get_ID(), m_pDigimon_Turn_Order[i]);
			}
			else
			{
				m_pTimeLine_array[i]->Set_Digimon(true, m_pDigimon_Turn_Order[i]->Get_ID(), m_pDigimon_Turn_Order[i]);

			}
		}

		// 이제 그 순서대로 deque에 넣어준다(위에서 넣었는데 또 넣는다고?)
		for (auto& Timeline : m_pTimeLine_array)
		{
			m_pTimeLines.push_back(Timeline);
		}

		// turn이라는 영어 글씨를 띄워주고
		m_pTurn->Set_Active(true);

		// 혹시 모르니 타임라인들을 다 true로 바꿔서 다 보이게 해준다.
		for (auto& Timeline : m_pTimeLines)
			Timeline->Set_Active(true);
	}
}

void CBattle_UI_Manager::Digimon_Dead()
{
}

void CBattle_UI_Manager::Turn_Start()
{
	if (m_bBattle)
	{
		m_pCurrent_TimeLine = m_pTimeLines.front();
		m_pTimeLines.pop_front();
		//m_pTimeLines.back()->Set_Active(true);
	}
}

void CBattle_UI_Manager::Turn_End()
{
	if (m_bBattle)
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
}

HRESULT CBattle_UI_Manager::Create_Skill()
{
	// 플레이어 디지몬의 갯수만큼 스킬창이랑 키보드를 만들어 준다.
	for (size_t i = 0; i < m_pPlayers.size(); ++i)
	{
		for (_int i = 0; i < 3; ++i)
		{
			m_pDigimon_Skill = static_cast<CBattle_Skill*>(m_pGameInstance->Add_GameObject_ToLayer_ToCreate(
				ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Battle_Skill"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Battle_Digimon_Skill")));
			if (m_pDigimon_Skill == nullptr)
				return E_FAIL;

			m_pKeyBord = static_cast<CKeyBord*>(m_pGameInstance->Add_GameObject_ToLayer_ToCreate(
				ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Battle_Skill_KayBord"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Battle_KeyBord")));
			if (m_pKeyBord == nullptr)
				return E_FAIL;


			m_pDigimon_Skills.push_back(m_pDigimon_Skill);
			m_pKeyBords.push_back(m_pKeyBord);
		}
	}
	return S_OK;
}

void CBattle_UI_Manager::Set_Skill()
{
	// 이제 만들었으면 위치를 다 잡아준다.
	m_pDigimon_Skills[0]->Set_Move(300, 650);
	m_pDigimon_Skills[0]->Set_Digimon_SkillSet(m_pPlayers[0]->Get_ID(), 0);
	m_pDigimon_Skills[0]->Set_Hover(true);
	m_pKeyBords[0]->Set_Move(300, 700);
	m_pKeyBords[0]->Set_KeyBord(0);
	m_pKeyBords[0]->Set_Hover(true);
	m_pDigimon_Skills[1]->Set_Move(365, 650);
	m_pDigimon_Skills[1]->Set_Digimon_SkillSet(m_pPlayers[0]->Get_ID(), 1);
	m_pKeyBords[1]->Set_Move(365, 700);
	m_pKeyBords[1]->Set_KeyBord(1);
	m_pDigimon_Skills[2]->Set_Move(430, 650);
	m_pDigimon_Skills[2]->Set_Digimon_SkillSet(m_pPlayers[0]->Get_ID(), 2);
	m_pKeyBords[2]->Set_Move(430, 700);
	m_pKeyBords[2]->Set_KeyBord(2);

	if (m_pPlayers.size() > 1)
	{
		m_pDigimon_Skills[3]->Set_Move(665, 650);
		m_pDigimon_Skills[3]->Set_Digimon_SkillSet(m_pPlayers[1]->Get_ID(), 0);
		m_pDigimon_Skills[3]->Set_Hover(true);
		m_pKeyBords[3]->Set_Move(665, 700);
		m_pKeyBords[3]->Set_KeyBord(3);
		m_pKeyBords[3]->Set_Hover(true);
		m_pDigimon_Skills[4]->Set_Move(730, 650);
		m_pDigimon_Skills[4]->Set_Digimon_SkillSet(m_pPlayers[1]->Get_ID(), 1);
		m_pKeyBords[4]->Set_Move(730, 700);
		m_pKeyBords[4]->Set_KeyBord(4);
		m_pDigimon_Skills[5]->Set_Move(795, 650);
		m_pDigimon_Skills[5]->Set_Digimon_SkillSet(m_pPlayers[1]->Get_ID(), 2);
		m_pKeyBords[5]->Set_Move(795, 700);
		m_pKeyBords[5]->Set_KeyBord(5);

	}

	if (m_pPlayers.size() > 2)
	{
		m_pDigimon_Skills[6]->Set_Move(1030, 650);
		m_pDigimon_Skills[6]->Set_Digimon_SkillSet(m_pPlayers[2]->Get_ID(), 0);
		m_pDigimon_Skills[6]->Set_Hover(true);
		m_pKeyBords[6]->Set_Move(1030, 700);
		m_pKeyBords[6]->Set_KeyBord(6);
		m_pKeyBords[6]->Set_Hover(true);
		m_pDigimon_Skills[7]->Set_Move(1095, 650);
		m_pDigimon_Skills[7]->Set_Digimon_SkillSet(m_pPlayers[2]->Get_ID(), 1);
		m_pKeyBords[7]->Set_Move(1095, 700);
		m_pKeyBords[7]->Set_KeyBord(7);
		m_pDigimon_Skills[8]->Set_Move(1160, 650);
		m_pDigimon_Skills[8]->Set_Digimon_SkillSet(m_pPlayers[2]->Get_ID(), 2);
		m_pKeyBords[8]->Set_Move(1160, 700);
		m_pKeyBords[8]->Set_KeyBord(8);
	}
}

void CBattle_UI_Manager::Delete_SKill()
{
	for (size_t i = 0; i < m_pKeyBords.size(); ++i)
	{
		Safe_Release(m_pKeyBords[i]);
	}
	for (size_t i = 0; i < m_pDigimon_Skills.size(); ++i)
	{
		Safe_Release(m_pDigimon_Skills[i]);
	}
	m_pKeyBords.clear();
	m_pDigimon_Skills.clear();
}

void CBattle_UI_Manager::Digimon_UseSkill1(_int SkillNum)
{
	if (m_bBattle)
	{
		_int Num = SkillNum - 1;
		m_iDigimon_Skill[0] = SkillNum;
		for (_int i = 0; i < 3; ++i)
		{
			if (Num == i)
				m_pDigimon_Skills[i]->Set_Hover(true);
			else
				m_pDigimon_Skills[i]->Set_Hover(false);
		}
	}
}

void CBattle_UI_Manager::Digimon_UseTarget1(_int Enemy)
{
	if (m_bBattle)
	{
		_int Num = Enemy - 1;

		for (_int i = 0; i < 3; ++i)
		{
			if (Num == i)
				m_pKeyBords[i]->Set_Hover(true);
			else
				m_pKeyBords[i]->Set_Hover(false);
		}
	}
}

void CBattle_UI_Manager::Digimon_UseSkill2(_int SkillNum)
{
	if (m_bBattle)
	{
		_int Num = SkillNum + 2;
		m_iDigimon_Skill[1] = Num;
		for (_int i = 3; i < 6; ++i)
		{
			if (Num == i)
				m_pDigimon_Skills[i]->Set_Hover(true);
			else
				m_pDigimon_Skills[i]->Set_Hover(false);
		}
	}
}

void CBattle_UI_Manager::Digimon_UseTarget2(_int Enemy)
{
	if (m_bBattle)
	{
		_int Num = Enemy + 2;
		for (_int i = 3; i < 6; ++i)
		{
			if (Num == i)
				m_pKeyBords[i]->Set_Hover(true);
			else
				m_pKeyBords[i]->Set_Hover(false);
		}
	}
}

void CBattle_UI_Manager::Digimon_UseSkill3(_int SkillNum)
{
	if (m_bBattle)
	{
		_int Num = SkillNum + 5;
		m_iDigimon_Skill[2] = SkillNum;
		for (_int i = 6; i < 9; ++i)
		{
			if (Num == i)
				m_pDigimon_Skills[i]->Set_Hover(true);
			else
				m_pDigimon_Skills[i]->Set_Hover(false);
		}
	}
}

void CBattle_UI_Manager::Digimon_UseTarget3(_int Enemy)
{
	if (m_bBattle)
	{
		_int Num = Enemy + 5;
		for (_int i = 6; i < 9; ++i)
		{
			if (Num == i)
				m_pKeyBords[i]->Set_Hover(true);
			else
				m_pKeyBords[i]->Set_Hover(false);
		}
	}
}

HRESULT CBattle_UI_Manager::CreateHp()
{
	// 이제 적 몬스터들의 체력바를 띄워 주는데 적의 갯수만큼 생성을 해준다.
	// 적이 최대 3마리인데 hp바의 위치를 조절하기 위해서 위치를 다 잡아주고
	for (int i = 0; i < m_iEnemyDigimonCount; ++i)
	{
		m_pEnemy_Hp = static_cast<CBattle_Enemy_Hp_BG*>(m_pGameInstance->Add_GameObject_ToLayer_ToCreate(
			ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Battle_Enemy_HpBG"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Battle_Enemy_Hp")));
		if (m_pEnemy_Hp == nullptr)
			return E_FAIL;

		m_pEnemyHps.push_back(m_pEnemy_Hp);
		m_iEnemy_Hp++;
	}

	m_iEnemy_HpPos[0] = _float2(450.f, 50.f);
	m_iEnemy_HpPos[1] = _float2(560.f, 50.f);
	m_iEnemy_HpPos[2] = _float2(670.f, 50.f);
	m_iEnemy_HpPos[3] = _float2(780.f, 50.f);
	m_iEnemy_HpPos[4] = _float2(890.f, 50.f);


	// 이제 적 디지몬의 갯수만큼 체력바를 배치 해준다.
	if (m_iEnemyDigimonCount == 1)
	{
		m_pEnemyHps[0]->Set_Move(m_iEnemy_HpPos[2].x, m_iEnemy_HpPos[2].y);
		m_pEnemyHps[0]->Set_Enemy(m_pEnemys[0]);
		m_pEnemyHps[0]->Set_Active(true);
	}
	else if (m_iEnemyDigimonCount == 2)
	{
		m_pEnemyHps[0]->Set_Move(m_iEnemy_HpPos[1].x, m_iEnemy_HpPos[1].y);
		m_pEnemyHps[0]->Set_Enemy(m_pEnemys[0]);
		m_pEnemyHps[0]->Set_Active(true);

		m_pEnemyHps[1]->Set_Move(m_iEnemy_HpPos[3].x, m_iEnemy_HpPos[3].y);
		m_pEnemyHps[1]->Set_Enemy(m_pEnemys[1]);
		m_pEnemyHps[1]->Set_Active(true);
	}
	else if (m_iEnemyDigimonCount == 3)
	{
		m_pEnemyHps[0]->Set_Move(m_iEnemy_HpPos[0].x, m_iEnemy_HpPos[0].y);
		m_pEnemyHps[0]->Set_Enemy(m_pEnemys[0]);
		m_pEnemyHps[0]->Set_Active(true);

		m_pEnemyHps[1]->Set_Move(m_iEnemy_HpPos[2].x, m_iEnemy_HpPos[2].y);
		m_pEnemyHps[1]->Set_Enemy(m_pEnemys[1]);
		m_pEnemyHps[1]->Set_Active(true);

		m_pEnemyHps[2]->Set_Move(m_iEnemy_HpPos[4].x, m_iEnemy_HpPos[4].y);
		m_pEnemyHps[2]->Set_Enemy(m_pEnemys[2]);
		m_pEnemyHps[2]->Set_Active(true);
	}

	return S_OK;
}

void CBattle_UI_Manager::Update_Enemy_HP(_int Num)
{
	if (m_bBattle)
	{
		if (m_pEnemyHps.size() == 2)
		{
			if (Num == 0)
			{
				m_pEnemyHps[0]->Set_Active(false);
				m_pEnemyHps[1]->Set_Move(m_iEnemy_HpPos[2].x, m_iEnemy_HpPos[2].y);
			}

			else if (Num == 1)
			{
				m_pEnemyHps[1]->Set_Active(false);
				m_pEnemyHps[0]->Set_Move(m_iEnemy_HpPos[2].x, m_iEnemy_HpPos[2].y);
			}
		}

		if (m_iEnemy_Hp == 3)
		{
			if (m_pEnemyHps.size() == 2)
			{
				if (Num == 0)
				{
					m_pEnemyHps[0]->Set_Active(false);
					m_pEnemyHps[1]->Set_Move(m_iEnemy_HpPos[2].x, m_iEnemy_HpPos[2].y);
				}

				else if (Num == 1)
				{
					m_pEnemyHps[1]->Set_Active(false);
					m_pEnemyHps[0]->Set_Move(m_iEnemy_HpPos[2].x, m_iEnemy_HpPos[2].y);
				}

				else if (Num == 2)
				{
					m_pEnemyHps[1]->Set_Active(false);
					m_pEnemyHps[0]->Set_Move(m_iEnemy_HpPos[2].x, m_iEnemy_HpPos[2].y);
				}
			}

			else if (m_pEnemyHps.size() == 3)
			{
				if (Num == 0)
				{
					m_pEnemyHps[0]->Set_Active(false);
					m_pEnemyHps[1]->Set_Move(m_iEnemy_HpPos[1].x, m_iEnemy_HpPos[1].y);
					m_pEnemyHps[2]->Set_Move(m_iEnemy_HpPos[3].x, m_iEnemy_HpPos[3].y);
					m_pEnemyHps.erase(m_pEnemyHps.begin());
				}

				else if (Num == 1)
				{
					m_pEnemyHps[1]->Set_Active(false);
					m_pEnemyHps[0]->Set_Move(m_iEnemy_HpPos[1].x, m_iEnemy_HpPos[1].y);
					m_pEnemyHps[2]->Set_Move(m_iEnemy_HpPos[3].x, m_iEnemy_HpPos[3].y);
					m_pEnemyHps.erase(m_pEnemyHps.begin() + 1);
				}
				else if (Num == 2)
				{
					m_pEnemyHps[2]->Set_Active(false);
					m_pEnemyHps[0]->Set_Move(m_iEnemy_HpPos[1].x, m_iEnemy_HpPos[1].y);
					m_pEnemyHps[1]->Set_Move(m_iEnemy_HpPos[3].x, m_iEnemy_HpPos[3].y);
					m_pEnemyHps.erase(m_pEnemyHps.begin() + 2);
				}
			}
		}

	}
}

void CBattle_UI_Manager::Delete_Hp()
{
	//m_pEnemy_Hp->Set_isDead(true);
	for (size_t i = 0; i < m_pEnemyHps.size(); ++i)
	{
		//m_pEnemyHps[i]->Set_isDead(true);
		Safe_Release(m_pEnemyHps[i]);
	}
	m_pEnemyHps.clear();
}

void CBattle_UI_Manager::Update_MyDigimon_Skill(CContainerObject* HitDigimon)
{
	if (m_bBattle)
	{
		for (size_t i = 0; i < m_pDigimon_Turn_Order.size(); ++i)
		{

		}
	}
}

void CBattle_UI_Manager::Update_TimeLine(CContainerObject* HitDigimon)
{
	if (m_bBattle)
	{
		for (size_t i = 0; i < m_pTimeLines.size(); ++i)
		{
			if (m_pTimeLines[i]->Get_Digimon() == HitDigimon)
			{
				m_pTimeLines[i]->Set_Move(_float2(1500.f, 0.f));
				m_pTimeLines.erase(m_pTimeLines.begin() + i);
				break;
			}
		}

		for (size_t i = m_iDeadDigimonNum; i < m_pTimeLines.size(); ++i)
		{
			m_pTimeLines[i]->Set_Move(m_fTurn_Panel[i]);
		}
	}
}

void CBattle_UI_Manager::Free()
{
	__super::Free();



	Safe_Release(m_pGameInstance);
}
