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

public:
	typedef struct DigimonOrder
	{
		_bool	m_bDigimonOrder = { false };
		_bool	m_bDigimonSkill = { false };
		_int	m_iDigimonSkill{};
		_int	m_iTarget{};
	}ORDER;

private:
	CBattle_Manager();
	virtual ~CBattle_Manager() = default;

public:
	HRESULT Initialize();
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);
public:
	void Player_Attack(_float fTimeDelta);
	_vector Player_Digimon_Attack_Pos(_int Target);

	void Enemy_Attack(_float fTimeDelta);
	_int Enemy_Skill();

	void Set_Turn();
	void Turn_Update();

	void Set_End(_bool End) { m_bEnemy_Death = End; }

	void ReturnToPosition(_float fTimeDelta);
	void Set_Player(CContainerObject* pPlayer);
	void Set_Digivice(CGameObject* pDigivice);
	void Set_Battle(CGameObject* pBattle);
	void Set_Battle_Terrain(CGameObject* pBattle_Terrain);

	void Gain_Experience(_int Exp);

	_bool Get_Battle() { return m_bBattleOn; }
	void Set_Battle(_bool Battle) {	m_bBattleOn = Battle; }

	void Battle_System();
	void Battle_End();
	void Return_Move();

	void Enemy_Position();
	void Player_Digimon_Position();
	void Current_Digimon(/*DIGIMON_INFO* Digimon_Info_1, DIGIMON_INFO* Digimon_Info_2, DIGIMON_INFO* Digimon_Info_3*/);

	_int CheckMana(_int DigimonNum,_int SkillNum);
	void Digimon1_Skill();
	void Digimon2_Skill();
	void Digimon3_Skill();

	void DigimonTargetOrder();

	void Digimon1_Attack(_float fTimeDelta);
	void Digimon2_Attack(_float fTimeDelta);
	void Digimon3_Attack(_float fTimeDelta);

	void EnemyDigimon_Info(_int EnemyDigimonID);

	CContainerObject* Digimon_Create(_int EnemyDigimonID);
	template<typename T>
	T* Digimon(const wstring& strPrototype);

	void Set_Player_Pos(_vector Pos);
	void Set_First_Digimon_Pos(_vector Pos);
	_vector Get_Player_Pos();

	void Set_Battle_Pos(_vector* vEmemyDigimon, _vector* vPlayerDigimon, _vector Player, _vector* vEnemyDigimonAttackPos, _vector* vPlayerDigimonAttackPos);

	void Set_MyDigimon(CContainerObject* pPlayerDigimon);

	void Battle_Turn_Order();

	void Digimon_Dead();
	void Digimon_Alive();

private:
	_bool							m_bBattleOn = { false };
	_int							m_iEnemyDigimonCount{};
	_int							m_iPlayerDigimonCount{};
	CGameInstance*					m_pGameInstance = { nullptr };

	class CDigimon_Manager*			m_pDigimon_Manager = { nullptr };
	class CBattle_UI_Manager*		m_pBattle_UI_Manager = { nullptr };
	class CInteraction_Manager*		m_pInteraction_Manager = { nullptr };

	vector<CContainerObject*>		m_pMyDigimon;
	vector<CContainerObject*>		m_pEnemyDigimon;
	vector<DIGIMON_INFO*>			m_pMyDigimon_Infos;

	CContainerObject*				m_pPlayer = { nullptr }; 
	CGameObject*					m_pDigivice = { nullptr };
	CGameObject*					m_pBattle_Terrain = { nullptr };
	CContainerObject*				m_pFirst_Digimon = { nullptr };


	_vector							m_pPlayer_Pos{};
	_vector							m_pFirst_Digimon_Pos{};

	_vector							m_vPlayerDigimonAttackLookAtEnemy{};
	_vector							m_vEnemyAttackLookAtPlayerDigimon{};

	_vector							m_vEnemyDigimonPos[5] = {};
	_vector							m_vPlayerDigimonPos[5] = {};
	_vector							m_vPlayerBattlePos{};

	_vector							m_vEnemyDigimonAttackPos[5] = {};
	_vector							m_vPlayerDigimonAttackPos[5] = {};

	_float							m_fEnemyDigimon_Look{};
	_float							m_fPlayer_Look{};

	_vector							m_pReturnPosition{};

	CGameObject*					m_pBattle = { nullptr };

	vector<CContainerObject*>		m_pDigimonSort;
	deque<CContainerObject*>		m_pDigimon_Turn_Order;


	_float							m_fBattleTime{};

	BATTLE_STATE					m_eBattle_State;

	CContainerObject*				m_pCurrentDigimon = { nullptr };
	CContainerObject*				m_pHitCurrentDigimon = { nullptr };


	_bool							m_bPlayer_Death = { false };
	_bool							m_bEnemy_Death = { false };

	_bool							m_bSkill = { false };
	_bool							m_bMove = { false };

	_float							m_fBackJumpTime{};
	_float							m_fDashTime{};

	_int							m_iLook_Target_position{};
	_int							m_iEnemyAttackSelect{};

	ORDER							m_DigimonOrder1;
	ORDER							m_DigimonOrder2;
	ORDER							m_DigimonOrder3;

	_int							m_iAlivePlayer{};
	_int							m_iAliveEnemy{};

	_int							m_iAdd_Exp{};
private:

public:
	virtual void Free() override;
};

NS_END