#include "Battle_Manager.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "ContainerObject.h"
#include "Digimon_Manager.h"
#include "Battle_UI_Manager.h"
#include "Interaction_Manager.h"
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
	m_pBattle_UI_Manager = CBattle_UI_Manager::GetInstance();
	m_pInteraction_Manager = CInteraction_Manager::GetInstance();
	m_bBattleOn = false;
	return S_OK;
}

void CBattle_Manager::Priority_Update(_float fTimeDelta)
{

}

void CBattle_Manager::Update(_float fTimeDelta)
{

	m_fBattleTime += fTimeDelta; // 시간 중첩
	if (m_bPlayer_Death || m_bEnemy_Death)
	{
		m_pBattle_UI_Manager->Battle_End();

		if (m_fBattleTime > 1.f)
			Battle_End();
	}

	else
	{
		Digimon1_Skill();
		Digimon2_Skill();
		Digimon3_Skill();
		DigimonTargetOrder();

		switch (m_eBattle_State)
		{
		case BATTLE_STATE::START:
			if (m_fBattleTime >= 1.f) // 1초가 지나면
			{
				m_pCurrentDigimon = m_pDigimon_Turn_Order.front();	// 제일 앞에 있는걸 저장하고
				m_pReturnPosition = m_pCurrentDigimon->Get_Position();
				m_fBackJumpTime = 0.f;
				m_pDigimon_Turn_Order.pop_front();					// 제잎 앞에꺼를 지움
				m_pBattle_UI_Manager->Turn_Start();
				m_eBattle_State = BATTLE_STATE::SKILL;
				m_fBattleTime = 0.f;							// 그리고 시간 초기화
			}

			break;

		case BATTLE_STATE::SKILL:
			m_fBattleTime = 0.f;									// 시간 초기화 

			if (m_pCurrentDigimon->Get_Life())						// 다시 살아있으면 실행
			{
				if (!m_pCurrentDigimon->Get_Monster())				// 플레이어 디지몬이면 플레이어 디지몬 공격 실행
				{
					Player_Attack(fTimeDelta);								// 스킬은 한번만 실행함
				}
				else												// 몬스터면 몬스터 실행
				{
					Enemy_Attack(fTimeDelta);
				}
			}
			if (!m_pCurrentDigimon->Get_SkillMove())
				m_eBattle_State = BATTLE_STATE::ING;
			else
			{
				m_fDashTime += fTimeDelta;
				m_eBattle_State = BATTLE_STATE::SKILL;
			}
			break;

		case BATTLE_STATE::ING:
			m_bSkill = true;
			if (m_pCurrentDigimon->Get_BackJump())
				ReturnToPosition(fTimeDelta);
			else
			{
				if (m_pCurrentDigimon->Get_TurnEnd())
				{
					m_eBattle_State = BATTLE_STATE::END;
				}
			}
			break;

		case BATTLE_STATE::END:
			if (!m_pCurrentDigimon->Get_Monster())
				m_pCurrentDigimon->LookAt(m_fPlayer_Look);
			else
				m_pCurrentDigimon->LookAt(m_fEnemyDigimon_Look);

			Turn_Update();
			m_fBattleTime = 0.f;									// 정확한 시간을 위해 0으로 초기화
			m_pBattle_UI_Manager->Turn_End();
			m_fDashTime = 0.f;
			m_eBattle_State = BATTLE_STATE::START;					// 처음으로 옮김
			m_bSkill = false;
			break;
		}
	}

}

void CBattle_Manager::Late_Update(_float fTimeDelta)
{

}

void CBattle_Manager::Player_Attack(_float fTimeDelta)
{
	if (!m_bSkill)
	{

		if (m_pCurrentDigimon == m_pMyDigimon[0])
		{
			if (m_DigimonOrder1.m_bDigimonSkill)
			{
				Digimon1_Attack(fTimeDelta);
				return;
			}
		}
		else if (m_pCurrentDigimon == m_pMyDigimon[1])
		{
			if (m_DigimonOrder2.m_bDigimonSkill)
			{
				Digimon2_Attack(fTimeDelta);
				return;
			}
		}
		else if (m_pCurrentDigimon == m_pMyDigimon[2])
		{
			if (m_DigimonOrder3.m_bDigimonSkill)
			{
				Digimon3_Attack(fTimeDelta);
				return;
			}
		}

		m_pCurrentDigimon->LookAt(Player_Digimon_Attack_Pos(1));
		m_pCurrentDigimon->UseSkill(1);
		m_pInteraction_Manager->Set_Hit_Digimon(m_pHitCurrentDigimon);
		m_bSkill = true;
	}

	if (m_pCurrentDigimon->Get_SkillMove())
	{
		m_fDashTime += fTimeDelta;
		if (m_fDashTime >= 0.1f)
		{
			m_pCurrentDigimon->LookAt(Player_Digimon_Attack_Pos(1));
			m_pCurrentDigimon->Target_Pos_Move(m_vPlayerDigimonAttackLookAtEnemy, fTimeDelta);
			if (!m_pCurrentDigimon->HasReachedTargetPosition(m_vPlayerDigimonAttackLookAtEnemy))
				m_pCurrentDigimon->Set_SkillMove(false);
		}
	}

}

_vector CBattle_Manager::Player_Digimon_Attack_Pos(_int Target)
{
	if (Target <= 0 || Target >= 4)
		Target = 1;

	if (Target == 1)
	{
		if (m_iEnemyDigimonCount == 1)
		{
			m_vPlayerDigimonAttackLookAtEnemy = m_vPlayerDigimonAttackPos[2];
			m_pHitCurrentDigimon = m_pEnemyDigimon[0];
			m_pCurrentDigimon->TargetPos(m_vEnemyDigimonPos[2]);
			return m_vEnemyDigimonPos[2];
		}

		else if (m_iEnemyDigimonCount == 2)
		{
			if (m_pEnemyDigimon[0]->Get_Die() == false)
			{
				m_vPlayerDigimonAttackLookAtEnemy = m_vPlayerDigimonAttackPos[1];
				m_pHitCurrentDigimon = m_pEnemyDigimon[0];
				m_pCurrentDigimon->TargetPos(m_vEnemyDigimonPos[1]);
				return m_vEnemyDigimonPos[1];
			}
			else
			{
				m_vPlayerDigimonAttackLookAtEnemy = m_vPlayerDigimonAttackPos[3];
				m_pHitCurrentDigimon = m_pEnemyDigimon[1];
				m_pCurrentDigimon->TargetPos(m_vEnemyDigimonPos[3]);
				return m_vEnemyDigimonPos[3];
			}
		}

		else if (m_iEnemyDigimonCount == 3)
		{
			if (m_pEnemyDigimon[0]->Get_Die() == false)
			{
				m_vPlayerDigimonAttackLookAtEnemy = m_vPlayerDigimonAttackPos[0];
				m_pHitCurrentDigimon = m_pEnemyDigimon[0];
				m_pCurrentDigimon->TargetPos(m_vEnemyDigimonPos[0]);
				return m_vEnemyDigimonPos[0];
			}
			else if (m_pEnemyDigimon[1]->Get_Die() == false)
			{
				m_vPlayerDigimonAttackLookAtEnemy = m_vPlayerDigimonAttackPos[2];
				m_pHitCurrentDigimon = m_pEnemyDigimon[1];
				m_pCurrentDigimon->TargetPos(m_vEnemyDigimonPos[2]);
				return m_vEnemyDigimonPos[2];
			}
			else
			{
				m_vPlayerDigimonAttackLookAtEnemy = m_vPlayerDigimonAttackPos[4];
				m_pHitCurrentDigimon = m_pEnemyDigimon[2];
				m_pCurrentDigimon->TargetPos(m_vEnemyDigimonPos[4]);
				return m_vEnemyDigimonPos[4];
			}
		}
	}

	if (Target == 2)
	{
		if (m_iEnemyDigimonCount == 1)
		{
			m_vPlayerDigimonAttackLookAtEnemy = m_vPlayerDigimonAttackPos[2];
			m_pHitCurrentDigimon = m_pEnemyDigimon[0];
			m_pCurrentDigimon->TargetPos(m_vEnemyDigimonPos[2]);
			return m_vEnemyDigimonPos[2];
		}

		else if (m_iEnemyDigimonCount == 2)
		{
			if (m_pEnemyDigimon[1]->Get_Die() == false)
			{
				m_vPlayerDigimonAttackLookAtEnemy = m_vPlayerDigimonAttackPos[3];
				m_pHitCurrentDigimon = m_pEnemyDigimon[1];
				m_pCurrentDigimon->TargetPos(m_vEnemyDigimonPos[3]);
				return m_vEnemyDigimonPos[3];
			}
			else
			{
				m_vPlayerDigimonAttackLookAtEnemy = m_vPlayerDigimonAttackPos[1];
				m_pHitCurrentDigimon = m_pEnemyDigimon[0];
				m_pCurrentDigimon->TargetPos(m_vEnemyDigimonPos[1]);
				return m_vEnemyDigimonPos[1];
			}
		}

		else if (m_iEnemyDigimonCount == 3)
		{
			if (m_pEnemyDigimon[1]->Get_Die() == false)
			{
				m_vPlayerDigimonAttackLookAtEnemy = m_vPlayerDigimonAttackPos[2];
				m_pHitCurrentDigimon = m_pEnemyDigimon[1];
				m_pCurrentDigimon->TargetPos(m_vEnemyDigimonPos[2]);
				return m_vEnemyDigimonPos[2];
			}
			else if (m_pEnemyDigimon[2]->Get_Die() == false)
			{
				m_vPlayerDigimonAttackLookAtEnemy = m_vPlayerDigimonAttackPos[4];
				m_pHitCurrentDigimon = m_pEnemyDigimon[2];
				m_pCurrentDigimon->TargetPos(m_vEnemyDigimonPos[4]);
				return m_vEnemyDigimonPos[4];
			}
			else
			{
				m_vPlayerDigimonAttackLookAtEnemy = m_vPlayerDigimonAttackPos[0];
				m_pHitCurrentDigimon = m_pEnemyDigimon[0];
				m_pCurrentDigimon->TargetPos(m_vEnemyDigimonPos[0]);
				return m_vEnemyDigimonPos[0];
			}
		}
	}

	if (Target == 3)
	{
		if (m_iEnemyDigimonCount == 1)
		{
			m_vPlayerDigimonAttackLookAtEnemy = m_vPlayerDigimonAttackPos[2];
			m_pHitCurrentDigimon = m_pEnemyDigimon[0];
			m_pCurrentDigimon->TargetPos(m_vEnemyDigimonPos[2]);
			return m_vEnemyDigimonPos[2];
		}

		else if (m_iEnemyDigimonCount == 2)
		{
			if (m_pEnemyDigimon[0]->Get_Die() == false)
			{
				m_vPlayerDigimonAttackLookAtEnemy = m_vPlayerDigimonAttackPos[1];
				m_pHitCurrentDigimon = m_pEnemyDigimon[0];
				m_pCurrentDigimon->TargetPos(m_vEnemyDigimonPos[1]);
				return m_vEnemyDigimonPos[1];
			}
			else
			{
				m_vPlayerDigimonAttackLookAtEnemy = m_vPlayerDigimonAttackPos[3];
				m_pHitCurrentDigimon = m_pEnemyDigimon[1];
				m_pCurrentDigimon->TargetPos(m_vEnemyDigimonPos[3]);
				return m_vEnemyDigimonPos[3];
			}
		}

		else if (m_iEnemyDigimonCount == 3)
		{
			if (m_pEnemyDigimon[2]->Get_Die() == false)
			{
				m_vPlayerDigimonAttackLookAtEnemy = m_vPlayerDigimonAttackPos[4];
				m_pHitCurrentDigimon = m_pEnemyDigimon[2];
				m_pCurrentDigimon->TargetPos(m_vEnemyDigimonPos[4]);
				return m_vEnemyDigimonPos[4];
			}
			else if (m_pEnemyDigimon[0]->Get_Die() == false)
			{
				m_vPlayerDigimonAttackLookAtEnemy = m_vPlayerDigimonAttackPos[0];
				m_pHitCurrentDigimon = m_pEnemyDigimon[0];
				m_pCurrentDigimon->TargetPos(m_vEnemyDigimonPos[0]);
				return m_vEnemyDigimonPos[0];
			}
			else
			{
				m_vPlayerDigimonAttackLookAtEnemy = m_vPlayerDigimonAttackPos[2];
				m_pHitCurrentDigimon = m_pEnemyDigimon[1];
				m_pCurrentDigimon->TargetPos(m_vEnemyDigimonPos[2]);
				return m_vEnemyDigimonPos[2];
			}
		}
	}

	return XMVectorSet(0.f, 0.f, 0.f, 0.f);
}

void CBattle_Manager::Enemy_Attack(_float fTimeDelta)
{
	if (m_iPlayerDigimonCount == 1)
	{
		m_iLook_Target_position = 2;
		m_iEnemyAttackSelect = 1;
		m_pHitCurrentDigimon = m_pMyDigimon[0];
		m_pCurrentDigimon->TargetPos(m_vPlayerDigimonPos[0]);
	}
	else if (m_iPlayerDigimonCount == 2)
	{
		if (m_pMyDigimon[0]->Get_Die() == false)
		{
			m_iLook_Target_position = 1;
			m_iEnemyAttackSelect = 1;
			m_pHitCurrentDigimon = m_pMyDigimon[0];
			m_pCurrentDigimon->TargetPos(m_vPlayerDigimonPos[0]);
		}
		else
		{
			m_iLook_Target_position = 3;
			m_iEnemyAttackSelect = 2;
			m_pHitCurrentDigimon = m_pMyDigimon[1];
			m_pCurrentDigimon->TargetPos(m_vPlayerDigimonPos[0]);
		}
	}
	else if (m_iPlayerDigimonCount == 3)
	{
		if (m_pMyDigimon[0]->Get_Die() == false)
		{
			m_iLook_Target_position = 0;
			m_iEnemyAttackSelect = 1;
			m_pHitCurrentDigimon = m_pMyDigimon[0];
			m_pCurrentDigimon->TargetPos(m_vPlayerDigimonPos[0]);
		}
		else if (m_pMyDigimon[1]->Get_Die() == false)
		{
			m_iLook_Target_position = 2;
			m_iEnemyAttackSelect = 2;
			m_pHitCurrentDigimon = m_pMyDigimon[1];
			m_pCurrentDigimon->TargetPos(m_vPlayerDigimonPos[0]);
		}
		else
		{
			m_iLook_Target_position = 4;
			m_iEnemyAttackSelect = 3;
			m_pHitCurrentDigimon = m_pMyDigimon[2];
			m_pCurrentDigimon->TargetPos(m_vPlayerDigimonPos[0]);
		}
	}


	if (!m_bSkill)
	{
		m_pCurrentDigimon->UseSkill(/*Enemy_Skill()*/3);
		m_pCurrentDigimon->LookAt(m_vPlayerDigimonPos[m_iLook_Target_position]);
		m_pInteraction_Manager->Set_Hit_Digimon(m_pHitCurrentDigimon);
		m_bSkill = true;
	}

	if (m_pCurrentDigimon->Get_SkillMove())
	{
		m_fDashTime += fTimeDelta;
		if (m_fDashTime >= 0.1f)
		{
			m_pCurrentDigimon->LookAt(m_vPlayerDigimonPos[m_iLook_Target_position]);
			m_pCurrentDigimon->Target_Pos_Move(XMVectorSet(m_vEnemyDigimonAttackPos[m_iLook_Target_position].m128_f32[0], 0.f, m_vEnemyDigimonAttackPos[m_iLook_Target_position].m128_f32[2], 1.f), fTimeDelta);
			if (!m_pCurrentDigimon->HasReachedTargetPosition(m_vEnemyDigimonAttackPos[m_iLook_Target_position]))
				m_pCurrentDigimon->Set_SkillMove(false);
		}
	}
}

_int CBattle_Manager::Enemy_Skill()
{
	_int Skill = m_pGameInstance->intRandom(1, 3);

	if (Skill == 1)
	{
		return 1;

	}
	else if (Skill == 2)
	{
		if (m_pCurrentDigimon->Get_Sp() < m_pCurrentDigimon->CurrentInfo().DigimonSkill2Info.Sp)
			return 1;
		else
			return 2;
	}
	else
	{
		if (m_pCurrentDigimon->Get_Sp() < m_pCurrentDigimon->CurrentInfo().DigimonSkill3Info.Sp)
			return 1;
		else
			return 3;
	}


}

void CBattle_Manager::Set_Turn()
{
	// 이제 다 세팅을 해줬으면 그 순서로 deque에 집어넣어 주고 매니저에 넘겨준다.
	for (_int i = 0; i < m_pDigimonSort.size(); ++i)
	{
		m_pDigimon_Turn_Order.push_back(m_pDigimonSort[i]);
	}

	m_pBattle_UI_Manager->Set_Battle_Turn_Order(m_pDigimon_Turn_Order);
}

void CBattle_Manager::Turn_Update()
{
	m_pDigimon_Turn_Order.push_back(m_pCurrentDigimon);
	m_pInteraction_Manager->Battle_End();

	if (m_pHitCurrentDigimon->Get_Die())
	{
		Digimon_Dead();
	}
	else
	{
		Digimon_Alive();
	}
}

void CBattle_Manager::ReturnToPosition(_float fTimeDelta)
{
	m_fBackJumpTime += fTimeDelta;
	if (m_fBackJumpTime >= 0.57f)
	{
		m_pCurrentDigimon->Target_Pos_Move(m_pReturnPosition, fTimeDelta * 1.8f);
		if (!m_pCurrentDigimon->HasReachedTargetPosition(m_pReturnPosition))
			m_pCurrentDigimon->Set_BackJump(false);
	}
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

void CBattle_Manager::Gain_Experience(_int Exp)
{
	m_iAdd_Exp += Exp;
}

void CBattle_Manager::Battle_System()
{
	m_pBattle_UI_Manager->Set_Battle(true);
	// 우선 플레이어 디지몬을 생성 해준다.
	//static_cast<CDigivice*>(m_pDigivice)->Update_Digimopn();
	Current_Digimon();
	// UI에 타임라인을 만들어 준다 이전에 적 몬스터를 다 만들어 놨고 아군 디지몬도 다 생성을 해서 그 갯수만큼 정보를 가지고 타임라인을 만든다.
	m_pBattle_UI_Manager->Create_TimeLine(m_iPlayerDigimonCount, m_iEnemyDigimonCount);
	// 이제 플레이어의 위치를 옮겨주고
	m_pPlayer->Set_Position(m_vPlayerBattlePos.m128_f32[0], m_vPlayerBattlePos.m128_f32[2]);
	// 아군이랑 적군 몬스터중 먼저 전멸하는 걸 확인하기 위해서 bool값을 2개 만들어 준다.
	m_bPlayer_Death = false;
	m_bEnemy_Death = false;
	// 전투의 상태도 start부터 시작하게 만들어 준다.
	m_eBattle_State = BATTLE_STATE::START;
	// 이제 내 디지몬들이랑 적 디지몬들의 위치를 조정 해준다.
	Player_Digimon_Position();
	Enemy_Position();
	// 전투의 순서를 정해준다.
	Battle_Turn_Order();

	// 이제 UI들을 다 정리 해준다.
	m_pBattle_UI_Manager->Set_Timeline_Turn_Order();
	m_pBattle_UI_Manager->CreateHp();
	m_pBattle_UI_Manager->Create_Skill();
	m_pBattle_UI_Manager->Set_Skill();
	// 내 디지몬들이랑 적 디지몬의 갯수도 저장 해준다.
	m_iAlivePlayer = m_iPlayerDigimonCount;
	m_iAliveEnemy = m_iEnemyDigimonCount;
}

void CBattle_Manager::Battle_End()
{
	m_pBattle_UI_Manager->Set_Battle(false);
	m_iAliveEnemy = m_iAlivePlayer = 0;

	for (size_t i = 0; i < m_pDigimon_Turn_Order.size(); ++i)
	{
		m_pDigimon_Turn_Order[i]->Set_Battle(false);
		if (m_pFirst_Digimon != m_pDigimon_Turn_Order[i])
			m_pDigimon_Turn_Order[i]->Set_isDead(true);
	}

	m_pMyDigimon.clear();
	m_pEnemyDigimon.clear();
	m_pMyDigimon_Infos.clear();
	m_pDigimonSort.clear();
	m_pDigimon_Turn_Order.clear();

	m_pBattle_Terrain = nullptr;
	m_pBattle = nullptr;
	m_pCurrentDigimon = nullptr;
	m_pHitCurrentDigimon = nullptr;

	m_bBattleOn = false;
	Return_Move();
}

void CBattle_Manager::Return_Move()
{
	m_pPlayer->Set_Position(m_pPlayer_Pos);
	m_pFirst_Digimon->Set_Position(m_pFirst_Digimon_Pos);
	m_pFirst_Digimon = nullptr;
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
		m_pEnemyDigimon.front()->Set_Position(m_vEnemyDigimonPos[1].m128_f32[0], m_vEnemyDigimonPos[1].m128_f32[2]);
		m_pEnemyDigimon.front()->LookAt(m_fEnemyDigimon_Look);
		m_pEnemyDigimon.front()->Set_Monster(true);
		m_pEnemyDigimon.front()->Set_Battle(true);
		m_pEnemyDigimon.back()->Set_Position(m_vEnemyDigimonPos[3].m128_f32[0], m_vEnemyDigimonPos[3].m128_f32[2]);
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
		m_pMyDigimon.front()->Set_Position(m_vPlayerDigimonPos[1].m128_f32[0], m_vPlayerDigimonPos[1].m128_f32[2]);
		m_pMyDigimon.front()->LookAt(m_fPlayer_Look);
		m_pMyDigimon.front()->Set_Battle(true);
		m_pMyDigimon.back()->Set_Position(m_vPlayerDigimonPos[3].m128_f32[0], m_vPlayerDigimonPos[3].m128_f32[2]);
		m_pMyDigimon.back()->LookAt(m_fPlayer_Look);
		m_pMyDigimon.back()->Set_Battle(true);
		break;
	case 3:
		for (_int i = 0; i < m_iPlayerDigimonCount; ++i)
		{
			_int j = i * 2;
			m_pMyDigimon[i]->Set_Position(m_vPlayerDigimonPos[j].m128_f32[0], m_vPlayerDigimonPos[j].m128_f32[2]);
			m_pMyDigimon[i]->LookAt(m_fPlayer_Look);
			m_pMyDigimon[i]->Set_Battle(true);
		}
		break;
	}
}

void CBattle_Manager::Current_Digimon(/*DIGIMON_INFO* Digimon_Info_1, DIGIMON_INFO* Digimon_Info_2, DIGIMON_INFO* Digimon_Info_3*/)
{
	// 전투가 시작이 되면 내 디지몬들을 최대 3마리 까지 만들고 m_pMyDigimon에 저장을 함
	// 전투가 끝나면 이 디지몬들을 세팅 해준다.
	m_iPlayerDigimonCount = 1;
	m_pFirst_Digimon = static_cast<CPlayer*>(m_pPlayer)->First_Digimon();
	m_pMyDigimon.push_back(static_cast<CPlayer*>(m_pPlayer)->First_Digimon());
	m_pMyDigimon[0]->Set_Digimon_Info(m_pDigimon_Manager->Get_Current_Digimon_Info(0));
	m_pBattle_UI_Manager->Set_MyDigimon(m_pMyDigimon[0]);
	m_pMyDigimon_Infos.push_back(&m_pMyDigimon[0]->CurrentInfo());
	if (m_pDigimon_Manager->Get_Current_Digimon_Info(1) != nullptr && m_pDigimon_Manager->Get_Current_Digimon_Info(1)->Hp != 0)
	{
		DIGIMON_INFO* Info = m_pDigimon_Manager->Get_Current_Digimon_Info(1);
		m_iPlayerDigimonCount++;
		m_pMyDigimon.push_back(Digimon_Create(Info->DigimonId));
		m_pMyDigimon[1]->Set_Digimon_Info(Info);
		m_pBattle_UI_Manager->Set_MyDigimon(m_pMyDigimon[1]);
		m_pMyDigimon_Infos.push_back(&m_pMyDigimon[1]->CurrentInfo());
	}

	if (m_pDigimon_Manager->Get_Current_Digimon_Info(2) != nullptr && m_pDigimon_Manager->Get_Current_Digimon_Info(1)->Hp != 0)
	{
		DIGIMON_INFO* Info = m_pDigimon_Manager->Get_Current_Digimon_Info(2);
		m_iPlayerDigimonCount++;
		m_pMyDigimon.push_back(Digimon_Create(Info->DigimonId));
		m_pMyDigimon[2]->Set_Digimon_Info(Info);
		m_pBattle_UI_Manager->Set_MyDigimon(m_pMyDigimon[2]);
		m_pMyDigimon_Infos.push_back(&m_pMyDigimon[2]->CurrentInfo());
	}
}

void CBattle_Manager::Digimon1_Skill()
{
	if (m_pGameInstance->Key_Down(DIK_Q))
	{
		m_DigimonOrder1.m_iDigimonSkill = 1;
		m_DigimonOrder1.m_bDigimonSkill = true;
		m_DigimonOrder1.m_bDigimonOrder = true;
		m_pBattle_UI_Manager->Digimon_UseSkill1(m_DigimonOrder1.m_iDigimonSkill);
	}

	if (m_pGameInstance->Key_Down(DIK_W))
	{
		m_DigimonOrder1.m_iDigimonSkill = 2;
		m_DigimonOrder1.m_bDigimonSkill = true;
		m_DigimonOrder1.m_bDigimonOrder = true;
		m_pBattle_UI_Manager->Digimon_UseSkill1(m_DigimonOrder1.m_iDigimonSkill);
	}

	if (m_pGameInstance->Key_Down(DIK_E))
	{
		m_DigimonOrder1.m_iDigimonSkill = 3;
		m_DigimonOrder1.m_bDigimonSkill = true;
		m_DigimonOrder1.m_bDigimonOrder = true;
		m_pBattle_UI_Manager->Digimon_UseSkill1(m_DigimonOrder1.m_iDigimonSkill);
	}
}

void CBattle_Manager::Digimon2_Skill()
{
	if (m_pGameInstance->Key_Down(DIK_A))
	{
		m_DigimonOrder2.m_iDigimonSkill = 1;
		m_DigimonOrder2.m_bDigimonSkill = true;
		m_DigimonOrder2.m_bDigimonOrder = true;
		m_pBattle_UI_Manager->Digimon_UseSkill2(m_DigimonOrder2.m_iDigimonSkill);
	}

	if (m_pGameInstance->Key_Down(DIK_S))
	{
		m_DigimonOrder2.m_iDigimonSkill = 2;
		m_DigimonOrder2.m_bDigimonSkill = true;
		m_DigimonOrder2.m_bDigimonOrder = true;
		m_pBattle_UI_Manager->Digimon_UseSkill2(m_DigimonOrder2.m_iDigimonSkill);
	}

	if (m_pGameInstance->Key_Down(DIK_D))
	{
		m_DigimonOrder2.m_iDigimonSkill = 3;
		m_DigimonOrder2.m_bDigimonSkill = true;
		m_DigimonOrder2.m_bDigimonOrder = true;
		m_pBattle_UI_Manager->Digimon_UseSkill2(m_DigimonOrder2.m_iDigimonSkill);
	}
}

void CBattle_Manager::Digimon3_Skill()
{
	if (m_pGameInstance->Key_Down(DIK_Z))
	{
		m_DigimonOrder3.m_iDigimonSkill = 1;
		m_DigimonOrder3.m_bDigimonSkill = true;
		m_DigimonOrder3.m_bDigimonOrder = true;
		m_pBattle_UI_Manager->Digimon_UseSkill3(m_DigimonOrder3.m_iDigimonSkill);
	}

	if (m_pGameInstance->Key_Down(DIK_X))
	{
		m_DigimonOrder3.m_iDigimonSkill = 2;
		m_DigimonOrder3.m_bDigimonSkill = true;
		m_DigimonOrder3.m_bDigimonOrder = true;
		m_pBattle_UI_Manager->Digimon_UseSkill3(m_DigimonOrder3.m_iDigimonSkill);
	}

	if (m_pGameInstance->Key_Down(DIK_C))
	{
		m_DigimonOrder3.m_iDigimonSkill = 3;
		m_DigimonOrder3.m_bDigimonSkill = true;
		m_DigimonOrder3.m_bDigimonOrder = true;
		m_pBattle_UI_Manager->Digimon_UseSkill3(m_DigimonOrder3.m_iDigimonSkill);
	}
}

void CBattle_Manager::DigimonTargetOrder()
{
	if (m_pGameInstance->Key_Down(DIK_1))
	{
		if (m_DigimonOrder1.m_bDigimonOrder)
		{
			m_DigimonOrder1.m_iTarget = 1;
			m_DigimonOrder1.m_bDigimonOrder = false;
			m_pBattle_UI_Manager->Digimon_UseTarget1(m_DigimonOrder1.m_iDigimonSkill);
		}
		if (m_DigimonOrder2.m_bDigimonOrder)
		{
			m_DigimonOrder2.m_iTarget = 1;
			m_DigimonOrder2.m_bDigimonOrder = false;
			m_pBattle_UI_Manager->Digimon_UseTarget2(m_DigimonOrder2.m_iDigimonSkill);
		}
		if (m_DigimonOrder3.m_bDigimonOrder)
		{
			m_DigimonOrder3.m_iTarget = 1;
			m_DigimonOrder3.m_bDigimonOrder = false;
			m_pBattle_UI_Manager->Digimon_UseTarget3(m_DigimonOrder3.m_iDigimonSkill);
		}
	}

	if (m_pGameInstance->Key_Down(DIK_2))
	{
		if (m_DigimonOrder1.m_bDigimonOrder)
		{
			m_DigimonOrder1.m_iTarget = 2;
			m_DigimonOrder1.m_bDigimonOrder = false;
			m_pBattle_UI_Manager->Digimon_UseTarget1(m_DigimonOrder1.m_iDigimonSkill);
		}
		if (m_DigimonOrder2.m_bDigimonOrder)
		{
			m_DigimonOrder2.m_iTarget = 2;
			m_DigimonOrder2.m_bDigimonOrder = false;
			m_pBattle_UI_Manager->Digimon_UseTarget2(m_DigimonOrder2.m_iDigimonSkill);
		}
		if (m_DigimonOrder3.m_bDigimonOrder)
		{
			m_DigimonOrder3.m_iTarget = 2;
			m_DigimonOrder3.m_bDigimonOrder = false;
			m_pBattle_UI_Manager->Digimon_UseTarget3(m_DigimonOrder3.m_iDigimonSkill);
		}
	}

	if (m_pGameInstance->Key_Down(DIK_3))
	{
		if (m_DigimonOrder1.m_bDigimonOrder)
		{
			m_DigimonOrder1.m_iTarget = 3;
			m_DigimonOrder1.m_bDigimonOrder = false;
			m_pBattle_UI_Manager->Digimon_UseTarget1(m_DigimonOrder1.m_iDigimonSkill);
		}
		if (m_DigimonOrder2.m_bDigimonOrder)
		{
			m_DigimonOrder2.m_iTarget = 3;
			m_DigimonOrder2.m_bDigimonOrder = false;
			m_pBattle_UI_Manager->Digimon_UseTarget2(m_DigimonOrder2.m_iDigimonSkill);
		}
		if (m_DigimonOrder3.m_bDigimonOrder)
		{
			m_DigimonOrder3.m_iTarget = 3;
			m_DigimonOrder3.m_bDigimonOrder = false;
			m_pBattle_UI_Manager->Digimon_UseTarget3(m_DigimonOrder3.m_iDigimonSkill);
		}
	}
}

void CBattle_Manager::Digimon1_Attack(_float fTimeDelta)
{
	m_pCurrentDigimon->LookAt(Player_Digimon_Attack_Pos(m_DigimonOrder1.m_iTarget));
	m_pCurrentDigimon->UseSkill(m_DigimonOrder1.m_iDigimonSkill);
	m_pInteraction_Manager->Set_Hit_Digimon(m_pHitCurrentDigimon);


	m_pBattle_UI_Manager->Digimon_UseSkill1(1);
	m_pBattle_UI_Manager->Digimon_UseTarget1(1);

	if (m_fDashTime >= 0.1f)
	{
		m_pCurrentDigimon->LookAt(Player_Digimon_Attack_Pos(m_DigimonOrder1.m_iTarget));
		m_pCurrentDigimon->Target_Pos_Move(m_vPlayerDigimonAttackLookAtEnemy, fTimeDelta);
		if (!m_pCurrentDigimon->HasReachedTargetPosition(m_vPlayerDigimonAttackLookAtEnemy))
			m_pCurrentDigimon->Set_SkillMove(false);
	}
}

void CBattle_Manager::Digimon2_Attack(_float fTimeDelta)
{
	m_pCurrentDigimon->LookAt(Player_Digimon_Attack_Pos(m_DigimonOrder2.m_iTarget));
	m_pCurrentDigimon->UseSkill(m_DigimonOrder2.m_iDigimonSkill);
	m_pInteraction_Manager->Set_Hit_Digimon(m_pHitCurrentDigimon);

	m_pBattle_UI_Manager->Digimon_UseSkill2(1);
	m_pBattle_UI_Manager->Digimon_UseTarget2(1);

	if (m_fDashTime >= 0.1f)
	{
		m_pCurrentDigimon->LookAt(Player_Digimon_Attack_Pos(m_DigimonOrder2.m_iTarget));
		m_pCurrentDigimon->Target_Pos_Move(m_vPlayerDigimonAttackLookAtEnemy, fTimeDelta);
		if (!m_pCurrentDigimon->HasReachedTargetPosition(m_vPlayerDigimonAttackLookAtEnemy))
			m_pCurrentDigimon->Set_SkillMove(false);
	}
}

void CBattle_Manager::Digimon3_Attack(_float fTimeDelta)
{
	m_pCurrentDigimon->LookAt(Player_Digimon_Attack_Pos(m_DigimonOrder3.m_iTarget));
	m_pCurrentDigimon->UseSkill(m_DigimonOrder3.m_iDigimonSkill);
	m_pInteraction_Manager->Set_Hit_Digimon(m_pHitCurrentDigimon);

	m_pBattle_UI_Manager->Digimon_UseSkill3(1);
	m_pBattle_UI_Manager->Digimon_UseTarget3(1);

	if (m_fDashTime >= 0.1f)
	{
		m_pCurrentDigimon->LookAt(Player_Digimon_Attack_Pos(m_DigimonOrder3.m_iTarget));
		m_pCurrentDigimon->Target_Pos_Move(m_vPlayerDigimonAttackLookAtEnemy, fTimeDelta);
		if (!m_pCurrentDigimon->HasReachedTargetPosition(m_vPlayerDigimonAttackLookAtEnemy))
			m_pCurrentDigimon->Set_SkillMove(false);


	}
}

void CBattle_Manager::EnemyDigimon_Info(_int EnemyDigimonID)
{
	// 적 디지몬을 생성하기 위해서 우선 안에 있는 값을 다 밀어버린다.
	m_pEnemyDigimon.clear();
	// 그 후에 적 디지몬의 갯수를 저장하고
	m_iEnemyDigimonCount = /*m_pGameInstance->intRandom(1, 3)*/3;
	for (_int i = 0; i < m_iEnemyDigimonCount; ++i)
	{
		m_pEnemyDigimon.push_back(Digimon_Create(EnemyDigimonID));

		m_pEnemyDigimon[i]->Set_Hp(1500);
		m_pEnemyDigimon[i]->Set_Sp(500);
		m_pEnemyDigimon[i]->Set_Damage(m_pGameInstance->intRandom(250, 500));
		m_pEnemyDigimon[i]->Set_AttackSpeed(10);
		m_pEnemyDigimon[i]->Set_Exp(m_pGameInstance->intRandom(900, 1000));
		m_pEnemyDigimon[i]->Set_Lv(m_pGameInstance->intRandom(1, 92));

		// UI에 디지몬의 정보를 넘겨준다.
		m_pBattle_UI_Manager->Set_Enemy_Digimon(m_pEnemyDigimon[i]);
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
T* CBattle_Manager::Digimon(const wstring& strPrototype)
{
	return   dynamic_cast<T*>(m_pGameInstance->Add_GameObject_ToLayer_ToCreate(
		ENUM_CLASS(LEVEL::GAMEPLAY), strPrototype, ENUM_CLASS(LEVEL::GAMEPLAY), L"Object_EnemyDigimon"));
}

void CBattle_Manager::Set_Player_Pos(_vector Pos)
{
	m_pPlayer_Pos = Pos;
}

void CBattle_Manager::Set_First_Digimon_Pos(_vector Pos)
{
	m_pFirst_Digimon_Pos = Pos;
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

void CBattle_Manager::Battle_Turn_Order()
{
	// 내 디지몬이랑 적 디지몬을 다 받아서 공격 속도를 비교한 다음에 우선 순위를 정해주는데 
	// 공격 속도가 똑같으면 이거를 계속 다르게 갱신 안하려고 stable_sort로 들어간 순서대로 먼저 오게한다.
	for (auto iter : m_pMyDigimon)
	{
		m_pDigimonSort.push_back(iter);
	}

	for (auto iter : m_pEnemyDigimon)
	{
		m_pDigimonSort.push_back(iter);
	}

	stable_sort(m_pDigimonSort.begin(), m_pDigimonSort.end(),
		[](CContainerObject* Src, CContainerObject* Dst)
		{return Src->Get_AttackSpeed() > Dst->Get_AttackSpeed(); });

	Set_Turn();
}

void CBattle_Manager::Digimon_Dead()
{
	if (m_pHitCurrentDigimon->Get_Monster() == true)
	{
		for (size_t i = 0; i < m_pEnemyDigimon.size(); ++i)
		{
			if (m_pHitCurrentDigimon == m_pEnemyDigimon[i])
			{
				m_pBattle_UI_Manager->Die_Enemy_HP(static_cast<int>(i));
				m_iAliveEnemy--;
				Gain_Experience(m_pHitCurrentDigimon->Get_Exp());
			}
		}
	}
	else
	{
		for (size_t i = 0; i < m_pMyDigimon.size(); ++i)
		{
			if (m_pHitCurrentDigimon == m_pMyDigimon[i])
			{
				m_pMyDigimon_Infos[i]->CurrentHp = 0;
				m_pBattle_UI_Manager->Update_MyDigimon_Skill(m_pHitCurrentDigimon);
				m_iAlivePlayer--;
			}
		}
	}

	m_pBattle_UI_Manager->Update_TimeLine(m_pHitCurrentDigimon);
	for (size_t i = 0; i < m_pDigimon_Turn_Order.size(); ++i)
	{
		if (m_pDigimon_Turn_Order[i] == m_pHitCurrentDigimon)
		{
			m_pDigimon_Turn_Order.erase(m_pDigimon_Turn_Order.begin() + i);
			break;
		}
	}
	m_pBattle_UI_Manager->Set_Battle_Turn_Order(m_pDigimon_Turn_Order);

	if (m_iAlivePlayer == 0)
		m_bPlayer_Death = true;
	if (m_iAliveEnemy == 0)
		m_bEnemy_Death = true;
}

void CBattle_Manager::Digimon_Alive()
{
	m_pBattle_UI_Manager->Set_Battle_Turn_Order(m_pDigimon_Turn_Order);
	if (m_pHitCurrentDigimon->Get_Monster())
	{
		m_pBattle_UI_Manager->Set_Hpbar(m_pHitCurrentDigimon, m_pCurrentDigimon->Get_Damage());
	}
	else
	{
		for (size_t i = 0; i < m_pMyDigimon.size(); ++i)
		{
			if (m_pHitCurrentDigimon == m_pMyDigimon[i])
			{
				m_pMyDigimon_Infos[i]->CurrentHp = m_pHitCurrentDigimon->Get_CurrentHp();
			}
		}
	}

	if (m_pCurrentDigimon == m_pMyDigimon[0])
	{
		m_DigimonOrder1.m_bDigimonSkill = false;
		return;
	}
	else if (m_pCurrentDigimon == m_pMyDigimon[1])
	{
		m_DigimonOrder2.m_bDigimonSkill = false;
		return;
	}
	else if (m_pCurrentDigimon == m_pMyDigimon[2])
	{
		m_DigimonOrder3.m_bDigimonSkill = false;
		return;
	}

}

void CBattle_Manager::Free()
{
	__super::Free();

	m_pMyDigimon.clear();
	m_pEnemyDigimon.clear();
	m_pMyDigimon_Infos.clear();
	m_pDigimonSort.clear();
	m_pDigimon_Turn_Order.clear();
}