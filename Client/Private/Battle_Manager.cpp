#include "Battle_Manager.h"


#include "GameInstance.h"
#include "GameObject.h"
#include "ContainerObject.h"
#include "Digimon_Manager.h"
#include "Battle_Terrain.h"

#include "Player.h"
#include "Digivice.h"

#include "Angewomon.h"
#include "Blackwargreymon.h"
#include "Devilmon.h"
#include "Ladydevimon.h"
#include "Leomon.h"
#include "Metalgarumon.h"
#include "Metalgreymon.h"
#include "Omegamon.h"
#include "Wargreymon.h"
#include "Beelzebumon.h"

IMPLEMENT_SINGLETON(CBattle_Manager);



CBattle_Manager::CBattle_Manager()
{
}

HRESULT CBattle_Manager::Initialize()
{
	m_fEnemyDigimon_Look = 0.f;
	m_fPlayer_Look = -180.f;

	m_pGameInstance = CGameInstance::GetInstance();
	m_pDigimon_Manager = CDigimon_Manager::GetInstance();
	return S_OK;
}

void CBattle_Manager::Priority_Update()
{
}

void CBattle_Manager::Update(_float fTimeDelta)
{
	if (m_bPlayer_Death || m_bEnemy_Death)							// 둘 중 하나라도 true면 베틀 종료
		m_bBattleOn = false;

	else
	{
		switch (m_eBattle_State)
		{
		case BATTLE_STATE::START:

			m_fBattleTime += fTimeDelta; // 시간 중첩

			if (m_fBattleTime >= 3.f) // 3초가 지나면
			{
				m_pCurrentDigimon = m_pDigimon_Turn_Order.front();	// 제일 앞에 있는걸 저장하고
				m_pDigimon_Turn_Order.pop_front();					// 제잎 앞에꺼를 지움

				if (m_pCurrentDigimon->Get_Life())					// 살아 있으면 ING로 넘어감
					m_eBattle_State = BATTLE_STATE::ING;
				else
				{
					m_eBattle_State = BATTLE_STATE::START;			// 죽으면 다시 처음으로 돌아감
					m_fBattleTime = 0.f;							// 그리고 시간 초기화
					Player_Set();
				}
			}

			break;

		case BATTLE_STATE::SKILL:
			m_fBattleTime = 0.f;									// 시간 초기화 

			if (m_pCurrentDigimon->Get_Life())						// 다시 살아있으면 실행
			{
				if (!m_pCurrentDigimon->Get_Monster())				// 플레이어 디지몬이면 플레이어 디지몬 공격 실행
				{
					Player_Attack();
				}
				else												// 몬스터면 몬스터 실행
				{
					Enemy_Attack();
				}
			}
			m_eBattle_State = BATTLE_STATE::ING;

			break;

		case BATTLE_STATE::ING:
			if (m_pCurrentDigimon->Get_TurnEnd())					// 애니메이션이 끝나면 END로 넘김
				m_eBattle_State = BATTLE_STATE::END;

			break;

		case BATTLE_STATE::END:
			m_pDigimon_Turn_Order.push_back(m_pCurrentDigimon);		// 제대로 끝이 나면 현재 공격했던 디지몬을 맨 뒤로 옮김
			m_fBattleTime = 0.f;									// 정확한 시간을 위해 0으로 초기화
			m_eBattle_State = BATTLE_STATE::START;					// 처음으로 옮김
			m_bSkill = false;
			break;
		}
	}
}

void CBattle_Manager::Late_Update()
{
}

void CBattle_Manager::Player_Attack()
{
}

void CBattle_Manager::Enemy_Attack()
{
	if (m_iPlayerDigimonCount == 1)
	{
		m_pCurrentDigimon->LookAt(m_vPlayerDigimonPos[2].m128_f32[0], m_vPlayerDigimonPos[2].m128_f32[1], m_vPlayerDigimonPos[2].m128_f32[2]);
	}
	else if (m_iPlayerDigimonCount == 2)
	{
		if(Digimon_1 != -1)
			m_pCurrentDigimon->LookAt(m_vPlayerDigimonPos[1].m128_f32[0], m_vPlayerDigimonPos[1].m128_f32[1], m_vPlayerDigimonPos[1].m128_f32[2]);
		else
			m_pCurrentDigimon->LookAt(m_vPlayerDigimonPos[3].m128_f32[0], m_vPlayerDigimonPos[3].m128_f32[1], m_vPlayerDigimonPos[3].m128_f32[2]);

	}
	else if (m_iPlayerDigimonCount == 3)
	{
		if (Digimon_1 != -1)
			m_pCurrentDigimon->LookAt(m_vPlayerDigimonPos[0].m128_f32[0], m_vPlayerDigimonPos[0].m128_f32[1], m_vPlayerDigimonPos[0].m128_f32[2]);
		else if(Digimon_2 != -1)
			m_pCurrentDigimon->LookAt(m_vPlayerDigimonPos[2].m128_f32[0], m_vPlayerDigimonPos[2].m128_f32[1], m_vPlayerDigimonPos[2].m128_f32[2]);
		else
			m_pCurrentDigimon->LookAt(m_vPlayerDigimonPos[4].m128_f32[0], m_vPlayerDigimonPos[4].m128_f32[1], m_vPlayerDigimonPos[4].m128_f32[2]);
	}
	if (!m_bSkill)
	{
		m_iSkill = m_pGameInstance->intRandom(1, 3);
		m_pCurrentDigimon->UseSkill(m_iSkill);
		m_bSkill = true;
	}

	if (m_pCurrentDigimon->Get_SkillMove()) 
	{
		//m_pCurrentDigimon->Set_Position()
	}


	//m_pDigimon_Turn_Order[Digimon_1];
}

void CBattle_Manager::Player_Set()
{
	Digimon_1 = Digimon_2 = Digimon_3 = -1;

	m_iEnemy_1 = m_iEnemy_2 = m_iEnemy_3 = -1;

	for (_int i = 0; i < m_pDigimonSort.size(); ++i)
	{
		m_pDigimon_Turn_Order.push_back(m_pDigimonSort[i]);

		if (!m_pDigimonSort[i]->Get_Monster())
		{
			if (Digimon_1 == -1)
				Digimon_1 = i;

			else if (Digimon_2 == -1)
				Digimon_2 = i;

			else if (Digimon_3 == -1)
				Digimon_3 = i;
		}
		else
		{
			if (m_iEnemy_1 == -1)
				m_iEnemy_1 = i;

			else if (m_iEnemy_2 == -1)
				m_iEnemy_2 = i;

			else if (m_iEnemy_3 == -1)
				m_iEnemy_3 = i;
		}
	}

	if (Digimon_1 == -1 && Digimon_2 == -1 && Digimon_3 == -1)
		m_bPlayer_Death = true;
	if (m_iEnemy_1 == -1 && m_iEnemy_2 == -1 && m_iEnemy_3 == -1)
		m_bEnemy_Death = true;
}

void CBattle_Manager::Set_Player(CContainerObject* pPlayer)
{
	m_pPlayer = pPlayer;
}

void CBattle_Manager::Set_Digivice(CGameObject* pDigivice)
{
	m_pDigivice = pDigivice;
}

void CBattle_Manager::Set_Battle(CGameObject* pBattle)
{
	m_pBattle = pBattle;
}

void CBattle_Manager::Set_Battle_Terrain(CGameObject* pBattle_Terrain)
{
	m_pBattle_Terrain = pBattle_Terrain;
}

void CBattle_Manager::Battle_System()
{
	m_pPlayer->Set_Position(m_vPlayerBattlePos.m128_f32[0], m_vPlayerBattlePos.m128_f32[2]);
	m_bPlayer_Death = false;
	m_bEnemy_Death	= false;
	m_eBattle_State = BATTLE_STATE::START;
	Enemy_Position();
	static_cast<CDigivice*>(m_pDigivice)->Update_Digimopn();
	Player_Digimon_Position();
	Battle_Tunr_Order();
}

void CBattle_Manager::Enemy_Position()
{
	switch (m_iEnemyDigimonCount)
	{
	case 1:
		m_pEnemyDigimon.front()->Set_Position(m_vEnemyDigimonPos[2].m128_f32[0], m_vEnemyDigimonPos[2].m128_f32[2]);
		m_pEnemyDigimon.front()->LookAt(m_fEnemyDigimon_Look);
		m_pEnemyDigimon.front()->Set_Monster(true);
		m_pEnemyDigimon.front()->Set_Battle(true);
		break;
	case 2:
		m_pEnemyDigimon.front()->Set_Position(m_vEnemyDigimonPos[1].m128_f32[0], m_vEnemyDigimonPos[3].m128_f32[2]);
		m_pEnemyDigimon.front()->LookAt(m_fEnemyDigimon_Look);
		m_pEnemyDigimon.front()->Set_Monster(true);
		m_pEnemyDigimon.front()->Set_Battle(true);
		m_pEnemyDigimon.back()->Set_Position(m_vEnemyDigimonPos[3].m128_f32[0], m_vEnemyDigimonPos[1].m128_f32[2]);
		m_pEnemyDigimon.back()->LookAt(m_fEnemyDigimon_Look);
		m_pEnemyDigimon.back()->Set_Monster(true);
		m_pEnemyDigimon.back()->Set_Battle(true);
		break;
	case 3:
		for (_int i = 0; i < m_iEnemyDigimonCount; ++i)
		{
			_int j = i * 2;
			m_pEnemyDigimon[i]->Set_Position(m_vEnemyDigimonPos[j].m128_f32[0], m_vEnemyDigimonPos[j].m128_f32[2]);
			m_pEnemyDigimon[i]->LookAt(m_fEnemyDigimon_Look);
			m_pEnemyDigimon[i]->Set_Monster(true);
			m_pEnemyDigimon[i]->Set_Battle(true);
		}
		break;
	}
}

void CBattle_Manager::Player_Digimon_Position()
{
	switch (m_iPlayerDigimonCount)
	{
	case 1:
		m_pMyDigimon.front()->Set_Position(m_vPlayerDigimonPos[2].m128_f32[0], m_vPlayerDigimonPos[2].m128_f32[2]);
		m_pMyDigimon.front()->LookAt(m_fPlayer_Look);
		m_pMyDigimon.front()->Set_Battle(true);
		break;
	case 2:
		m_pMyDigimon.front()->Set_Position(m_vPlayerDigimonPos[1].m128_f32[0], m_vPlayerDigimonPos[3].m128_f32[2]);
		m_pMyDigimon.front()->LookAt(m_fPlayer_Look);
		m_pMyDigimon.front()->Set_Battle(true);
		m_pMyDigimon.back()->Set_Position(m_vPlayerDigimonPos[3].m128_f32[0], m_vPlayerDigimonPos[1].m128_f32[2]);
		m_pMyDigimon.back()->LookAt(m_fPlayer_Look);
		m_pMyDigimon.back()->Set_Battle(true);
		break;
	case 3:
		for (_int i = 0; i < m_iEnemyDigimonCount; ++i)
		{
			_int j = i * 2;
			m_pMyDigimon[i]->Set_Position(m_vPlayerDigimonPos[j].m128_f32[0], m_vPlayerDigimonPos[j].m128_f32[2]);
			m_pMyDigimon[i]->LookAt(m_fPlayer_Look);
			m_pMyDigimon[i]->Set_Battle(true);
		}
		break;
	}
}

void CBattle_Manager::Current_Digimon(_int ID1, _int ID2, _int ID3)
{
	m_iPlayerDigimonCount = 1;
	m_pMyDigimon.push_back(static_cast<CPlayer*>(m_pPlayer)->First_Digimon());
	if (ID2 != -1)
	{
		m_iPlayerDigimonCount++;
		m_pMyDigimon.push_back(Digimon_Create(ID2));
	}

	if (ID3 != -1)
	{
		m_iPlayerDigimonCount++;
		m_pMyDigimon.push_back(Digimon_Create(ID3));
	}
}

void CBattle_Manager::Digimon1_Skill(_int ID)
{
	if (m_pGameInstance->Key_Down(DIK_Q))
	{
		m_pMyDigimon[ID]->UseSkill(1);
	}

	if (m_pGameInstance->Key_Down(DIK_W))
	{
		m_pMyDigimon[ID]->UseSkill(2);

	}

	if (m_pGameInstance->Key_Down(DIK_E))
	{
		m_pMyDigimon[ID]->UseSkill(3);
	}
}

void CBattle_Manager::Digimon2_Skill(_int ID)
{
	if (m_pGameInstance->Key_Down(DIK_A))
	{
		m_pMyDigimon[ID]->UseSkill(1);
	}

	if (m_pGameInstance->Key_Down(DIK_S))
	{
		m_pMyDigimon[ID]->UseSkill(2);
	}

	if (m_pGameInstance->Key_Down(DIK_D))
	{
		m_pMyDigimon[ID]->UseSkill(3);
	}
}

void CBattle_Manager::Digimon3_Skill(_int ID)
{
	if (m_pGameInstance->Key_Down(DIK_Z))
	{
		m_pMyDigimon[ID]->UseSkill(1);
	}

	if (m_pGameInstance->Key_Down(DIK_X))
	{
		m_pMyDigimon[ID]->UseSkill(2);
	}

	if (m_pGameInstance->Key_Down(DIK_C))
	{
		m_pMyDigimon[ID]->UseSkill(3);
	}
}

void CBattle_Manager::EnemyDigimon_Info(_int EnemyDigimonID)
{
	m_pEnemyDigimon.clear();
	m_iEnemyDigimonCount = m_pGameInstance->intRandom(1, 3);
	for (_int i = 0; i < m_iEnemyDigimonCount; ++i)
	{
		m_pEnemyDigimon.push_back(Digimon_Create(EnemyDigimonID));

		m_pEnemyDigimon[i]->Set_Hp(m_pGameInstance->intRandom(500, 3000));
		m_pEnemyDigimon[i]->Set_Sp(m_pGameInstance->intRandom(30, 100));
		m_pEnemyDigimon[i]->Set_Damage(m_pGameInstance->intRandom(250, 500));
		m_pEnemyDigimon[i]->Set_AttackSpeed(m_pGameInstance->intRandom(1, 3));
		m_pEnemyDigimon[i]->Set_Exp(m_pGameInstance->intRandom(900, 1000));
		m_pEnemyDigimon[i]->Set_Lv(m_pGameInstance->intRandom(1, 92));
	}
}

CContainerObject* CBattle_Manager::Digimon_Create(_int EnemyDigimonID)
{
	wstring DigimonModel = m_pDigimon_Manager->Get_Prototype_ID(EnemyDigimonID);
	switch (EnemyDigimonID)
	{
	case 0:
		return Digimon<CAngewomon>(DigimonModel);
		break;

	case 1:
		return Digimon<CBlackwargreymon>(DigimonModel);
		break;

	case 2:
		return Digimon<CDevilmon>(DigimonModel);
		break;

	case 3:
		return Digimon<CLadydevimon>(DigimonModel);
		break;

	case 4:
		return Digimon<CLeomon>(DigimonModel);
		break;

	case 5:
		return Digimon<CMetalgarumon>(DigimonModel);
		break;

	case 6:
		return Digimon<CMetalgreymon>(DigimonModel);
		break;

	case 7:
		return Digimon<COmegamon>(DigimonModel);
		break;

	case 8:
		return Digimon<CWargreymon>(DigimonModel);
		break;

	case 9:
		return Digimon<CBeelzebumon>(DigimonModel);
		break;
	}

	return nullptr;
}

template<typename T>
CContainerObject* CBattle_Manager::Digimon(const wstring& strPrototype)
{
	CContainerObject* m_pDigimon = { nullptr };
	m_pDigimon = dynamic_cast<T*>(m_pGameInstance->Add_GameObject_ToLayer_ToCreate(ENUM_CLASS(LEVEL::GAMEPLAY), strPrototype, ENUM_CLASS(LEVEL::GAMEPLAY), L"Object_EnemyDigimon"));

	return  m_pDigimon;
}

void CBattle_Manager::Set_Player_Pos(_vector Pos)
{
	m_pPlayer_Pos = Pos;
}

_vector CBattle_Manager::Get_Player_Pos()
{
	return m_pPlayer_Pos;
}

void CBattle_Manager::Set_Battle_Pos(_vector* vEmemyDigimon, _vector* vPlayerDigimon, _vector Player, _vector* vEnemyDigimonAttackPos, _vector* vPlayerDigimonAttackPos)
{
	for (_int i = 0; i < 5; ++i)
	{
		m_vEnemyDigimonPos[i] = vEmemyDigimon[i];
		m_vPlayerDigimonPos[i] = vPlayerDigimon[i];
		m_vEnemyDigimonAttackPos[i] = vEnemyDigimonAttackPos[i];
		m_vPlayerDigimonAttackPos[i] = vPlayerDigimonAttackPos[i];
	}
	m_vPlayerBattlePos = Player;


}

void CBattle_Manager::Set_MyDigimon(CContainerObject* pPlayerDigimon)
{
	m_pMyDigimon.push_back(pPlayerDigimon);
}

void CBattle_Manager::Battle_Tunr_Order()
{
	for (auto iter : m_pMyDigimon)
	{
		m_pDigimonSort.push_back(iter);
	}

	for (auto iter : m_pEnemyDigimon)
	{
		m_pDigimonSort.push_back(iter);
	}

	sort(m_pDigimonSort.begin(), m_pDigimonSort.end(),
		[](CContainerObject* Src, CContainerObject* Dst)
		{return Src->Get_AttackSpeed() > Dst->Get_AttackSpeed(); });

	Player_Set();
}

void CBattle_Manager::Free()
{
	__super::Free();

	m_pMyDigimon.clear();
	m_pEnemyDigimon.clear();
}
