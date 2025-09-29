#pragma once

#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;
class CContainerObject;
class CGameObject;
NS_END

NS_BEGIN(Client)
class CBattle_Manager final : public CBase
{
public:
	DECLARE_SINGLETON(CBattle_Manager);
private:
	CBattle_Manager();
	virtual ~CBattle_Manager() = default;

public:
	HRESULT Initialize();
	void Priority_Update();
	void Update(_float fTimeDelta);
	void Late_Update();
public:
	void Player_Attack(_float fTimeDelta);
	void Player_Digimon_Attack_Pos(_int Target);

	void Enemy_Attack(_float fTimeDelta);
	void Player_Set();


	void Set_Player(CContainerObject* pPlayer);
	void Set_Digivice(CGameObject* pDigivice);
	void Set_Battle(CGameObject* pBattle);
	void Set_Battle_Terrain(CGameObject* pBattle_Terrain);

	_bool Get_Battle() { return m_bBattleOn; }
	void Set_Battle(_bool Battle) {	m_bBattleOn = Battle; }

	void Battle_System();

	void Enemy_Position();
	void Player_Digimon_Position();
	void Current_Digimon(_int ID1, _int ID2, _int ID3);

	void Digimon1_Skill(_int ID);
	void Digimon2_Skill(_int ID);
	void Digimon3_Skill(_int ID);

	void EnemyDigimon_Info(_int EnemyDigimonID);

	CContainerObject* Digimon_Create(_int EnemyDigimonID);
	template<typename T>
	CContainerObject* Digimon(const wstring& strPrototype);

	void Set_Player_Pos(_vector Pos);
	_vector Get_Player_Pos();

	void Set_Battle_Pos(_vector* vEmemyDigimon, _vector* vPlayerDigimon, _vector Player, _vector* vEnemyDigimonAttackPos, _vector* vPlayerDigimonAttackPos);

	void Set_MyDigimon(CContainerObject* pPlayerDigimon);

	void Battle_Tunr_Order();

private:
	_bool							m_bBattleOn = { false };
	_int							m_iEnemyDigimonCount{};
	_int							m_iPlayerDigimonCount{};
	CGameInstance*					m_pGameInstance = { nullptr };

	class CDigimon_Manager*			m_pDigimon_Manager = { nullptr };

	vector<CContainerObject*>		m_pMyDigimon;
	vector<CContainerObject*>		m_pEnemyDigimon = { nullptr };

	CContainerObject*				m_pPlayer = { nullptr };
	CGameObject*					m_pDigivice = { nullptr };

	CGameObject*					m_pBattle_Terrain = { nullptr };

	_vector							m_pPlayer_Pos{};

	_vector							m_vEnemyDigimonPos[5] = {};
	_vector							m_vPlayerDigimonPos[5] = {};
	_vector							m_vPlayerBattlePos{};

	_vector							m_vEnemyDigimonAttackPos[5] = {};
	_vector							m_vPlayerDigimonAttackPos[5] = {};

	_float							m_fEnemyDigimon_Look{};
	_float							m_fPlayer_Look{};

	CGameObject*					m_pBattle = { nullptr };

	vector<CContainerObject*>		m_pDigimonSort;
	deque<CContainerObject*>		m_pDigimon_Turn_Order;

	_int							Digimon_1{};
	_int							Digimon_2{};
	_int							Digimon_3{};

	_int							m_iEnemy_1{};
	_int							m_iEnemy_2{};
	_int							m_iEnemy_3{};

	_float							m_fBattleTime{};

	BATTLE_STATE					m_eBattle_State;

	CContainerObject*				m_pCurrentDigimon = { nullptr };

	_bool							m_bPlayer_Death = { false };
	_bool							m_bEnemy_Death = { false };

	_int							m_iSkill;
	_bool							m_bSkill = { false };
	_bool							m_bMove = { false };


private:

public:
	virtual void Free() override;
};

NS_END