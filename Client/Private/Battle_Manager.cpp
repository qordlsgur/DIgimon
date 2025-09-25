#include "Battle_Manager.h"


#include "GameInstance.h"
#include "GameObject.h"
#include "ContainerObject.h"
#include "Digimon_Manager.h"
#include "Battle_Terrain.h"

#include "Angewomon.h"
#include "Blackwargreymon.h"
#include "Devilmon.h"
#include "Ladydevimon.h"
#include "Leomon.h"
#include "Metalgarumon.h"
#include "Metalgreymon.h"
#include "Omegamon.h"
#include "Wargreymon.h"

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

void CBattle_Manager::Update()
{
}

void CBattle_Manager::Late_Update()
{
}

void CBattle_Manager::Set_Player(CContainerObject* pPlayer)
{
	m_pPlayer = pPlayer;
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

	switch (m_iEnemyDigimonCount)
	{
	case 1:
		m_pEnemyDigimon.front()->Set_Position(m_vMnemyDigimonPos[2].m128_f32[0], m_vMnemyDigimonPos[2].m128_f32[2]);
		m_pEnemyDigimon.front()->LookAt(m_fEnemyDigimon_Look);
		break;
	case 2:
		m_pEnemyDigimon.front()->Set_Position(m_vMnemyDigimonPos[1].m128_f32[0], m_vMnemyDigimonPos[3].m128_f32[2]);
		m_pEnemyDigimon.front()->LookAt(m_fEnemyDigimon_Look);
		m_pEnemyDigimon.back()->Set_Position(m_vMnemyDigimonPos[3].m128_f32[0], m_vMnemyDigimonPos[1].m128_f32[2]);
		m_pEnemyDigimon.back()->LookAt(m_fEnemyDigimon_Look);
		break;
	case 3:
		for (_int i = 0; i < m_iEnemyDigimonCount; ++i)
		{
			_int j = i * 2;
			m_pEnemyDigimon[i]->Set_Position(m_vMnemyDigimonPos[j].m128_f32[0], m_vMnemyDigimonPos[j].m128_f32[2]);
			m_pEnemyDigimon[i]->LookAt(m_fEnemyDigimon_Look);
		}
		break;
	}

}

void CBattle_Manager::Digimon1_Skill()
{
	if (m_pGameInstance->Key_Down(DIK_Q))
	{
		m_pMyDigimon[0]->Skill1();
	}

	if (m_pGameInstance->Key_Down(DIK_W))
	{
		m_pMyDigimon[0]->Skill2();
	}

	if (m_pGameInstance->Key_Down(DIK_E))
	{
		m_pMyDigimon[0]->Skill3();
	}
}

void CBattle_Manager::Digimon2_Skill()
{
	if (m_pGameInstance->Key_Down(DIK_A))
	{
		m_pMyDigimon[1]->Skill1();
	}

	if (m_pGameInstance->Key_Down(DIK_S))
	{
		m_pMyDigimon[1]->Skill1();
	}

	if (m_pGameInstance->Key_Down(DIK_D))
	{
		m_pMyDigimon[1]->Skill1();
	}
}

void CBattle_Manager::Digimon3_Skill()
{
	if (m_pGameInstance->Key_Down(DIK_Z))
	{
		m_pMyDigimon[2]->Skill1();
	}

	if (m_pGameInstance->Key_Down(DIK_X))
	{
		m_pMyDigimon[2]->Skill2();
	}

	if (m_pGameInstance->Key_Down(DIK_C))
	{
		m_pMyDigimon[2]->Skill3();
	}
}

void CBattle_Manager::EnemyDigimon_Info(_int EnemyDigimonID)
{
	m_pEnemyDigimon.clear();
	m_iEnemyDigimonCount = m_pGameInstance->intRandom(1, 3);
	for (_int i = 0; i < m_iEnemyDigimonCount; ++i)
	{
		m_pEnemyDigimon.push_back(EnemyDigimon_Create(EnemyDigimonID));
	}
}

CContainerObject* CBattle_Manager::EnemyDigimon_Create(_int EnemyDigimonID)
{
	wstring DigimonModel = m_pDigimon_Manager->Get_Prototype_ID(EnemyDigimonID);
	switch (EnemyDigimonID)
	{
	case 0:
		return EnemyDigimon<CAngewomon>(DigimonModel);
		break;

	case 1:
		return EnemyDigimon<CBlackwargreymon>(DigimonModel);
		break;

	case 2:
		return EnemyDigimon<CDevilmon>(DigimonModel);
		break;

	case 3:
		return EnemyDigimon<CLadydevimon>(DigimonModel);
		break;

	case 4:
		return EnemyDigimon<CLeomon>(DigimonModel);
		break;

	case 5:
		return EnemyDigimon<CMetalgarumon>(DigimonModel);
		break;

	case 6:
		return EnemyDigimon<CMetalgreymon>(DigimonModel);
		break;

	case 7:
		return EnemyDigimon<COmegamon>(DigimonModel);
		break;

	case 8:
		return EnemyDigimon<CWargreymon>(DigimonModel);
		break;

	case 9:
		return EnemyDigimon<CAngewomon>(DigimonModel);
		break;
	}

	return nullptr;
}

template<typename T>
CContainerObject* CBattle_Manager::EnemyDigimon(const wstring& strPrototype)
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

void CBattle_Manager::Set_Battle_Pos(_vector* vEmemyDigimon, _vector* vPlayerDigimon, _vector Player)
{
	for (_int i = 0; i < 5; ++i)
	{
		m_vMnemyDigimonPos[i] = vEmemyDigimon[i];
		m_vPlayerDigimonPos[i] = vPlayerDigimon[i];
	}
	m_vPlayerBattlePos = Player;
}

void CBattle_Manager::Set_MyDigimon(CContainerObject* pPlayerDigimon)
{
	m_pMyDigimon.push_back(pPlayerDigimon);
}

void CBattle_Manager::Free()
{
	__super::Free();

	m_pMyDigimon.clear();
}
