#pragma once

#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Engine)
class CGameObject;
class CContainerObject;
class CGameInstance;
NS_END

NS_BEGIN(Client)

class CBattle_UI_Manager final : public CBase
{
public:
	DECLARE_SINGLETON(CBattle_UI_Manager);

private:
	CBattle_UI_Manager();
	virtual ~CBattle_UI_Manager() = default;

public:
	HRESULT Initialize();

public:
	HRESULT Create_TimeLine(_int MyDigimonCount, _int EnemyDigimonCount);
	void	Battle_End();

	void Set_Battle_Turn_Order(deque<CContainerObject*>Digimon_Turn_Order);
	void Set_Hpbar(CContainerObject* Enemy, _int Damage);

	void Set_MyDigimon(CContainerObject* MyDigimon);
	void Set_Enemy_Digimon(CContainerObject* EnemyDigimon);

	void Set_Timeline_Turn_Order();

	void Digimon_Dead();

	void Turn_Start();
	void Turn_End();

	HRESULT Create_Skill();
	void Set_Skill();

	void Digimon_UseSkill1(_int SkillNum);
	void Digimon_UseTarget1(_int Enemy);
	void Digimon_UseSkill2(_int SkillNum);
	void Digimon_UseTarget2(_int Enemy);
	void Digimon_UseSkill3(_int SkillNum);
	void Digimon_UseTarget3(_int Enemy);

	HRESULT CreateHp();
	void Update_Enemy_HP(_int Num);

	void Update_MyDigimon_Skill(CContainerObject* HitDigimon);
	void Update_TimeLine(CContainerObject* HitDigimon);

	void Set_Battle(_bool Battle) { m_bBattle = Battle;	}

private:
	CGameInstance* m_pGameInstance = { nullptr };
	_int m_iMyDigimonCount{};
	_int m_iEnemyDigimonCount{};

	deque<CContainerObject*>		m_pDigimon_Turn_Order;

	vector<CContainerObject*>	m_pPlayers;
	vector<CContainerObject*>	m_pEnemys;

	//vector<DIGIMON_INFO*>		m_MyDigimon_Info;
	//vector<DIGIMON_INFO*>		m_EnemyDigimon_Info;

	deque<class CBattle_Timeline*>	m_pTimeLines;
	vector<class CBattle_Timeline*> m_pTimeLine_array;
	class CBattle_Timeline* m_pTimeLine = { nullptr };
	class CBattle_Turn* m_pTurn = { nullptr };
	class CBattle_Timeline* m_pCurrent_TimeLine = { nullptr };
	class CBattle_Skill* m_pDigimon_Skill = { nullptr };
	class CKeyBord* m_pKeyBord = { nullptr };
	class CBattle_Enemy_Hp_BG* m_pEnemy_Hp = { nullptr };

	vector<class CBattle_Skill*> m_pDigimon_Skills;
	vector<class CKeyBord*> m_pKeyBords;
	vector<class CBattle_Enemy_Hp_BG*> m_pEnemyHps;

	_float2 m_fTurn_Panel[6];
	_int m_iDigimon{};
	_int m_iDigimon_Skill[3] = {};

	_float2 m_iEnemy_HpPos[5] = {};
	_int m_iEnemy_Hp{};

	_int m_iDeadDigimonNum{};

	_bool	m_bBattle = { false };

public:
	virtual void Free() override;
};

NS_END