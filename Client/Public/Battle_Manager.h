#pragma once

#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;
class CGameObject;
class CContainerObject;
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
	void Update();
	void Late_Update();
public:
	void Set_Player(CContainerObject* pPlayer);
	void Set_Battle(CGameObject* pBattle);
	void Set_Battle_Terrain(CGameObject* pBattle_Terrain);

	_bool Get_Battle() { return m_bBattleOn; }
	void Set_Battle(_bool Battle) {	m_bBattleOn = Battle; }

	void Battle_System();

	void Digimon1_Skill();
	void Digimon2_Skill();
	void Digimon3_Skill();

	void EnemyDigimon_Info(_int EnemyDigimonID);

	CContainerObject* EnemyDigimon_Create(_int EnemyDigimonID);
	template<typename T>
	CContainerObject* EnemyDigimon(const wstring& strPrototype);

	void Set_Player_Pos(_vector Pos);
	_vector Get_Player_Pos();

	void Set_Battle_Pos(_vector* vEmemyDigimon, _vector* vPlayerDigimon, _vector Player);

	void Set_MyDigimon(CContainerObject* pPlayerDigimon);
private:
	_bool							m_bBattleOn = { false };
	_int							m_iEnemyDigimonCount;
	CGameInstance*					m_pGameInstance = { nullptr };

	class CDigimon_Manager*			m_pDigimon_Manager = { nullptr };

	vector<CContainerObject*>		m_pMyDigimon;
	vector<CContainerObject*>		m_pEnemyDigimon = { nullptr };
	CContainerObject*				m_pPlayer = { nullptr };

	CGameObject*					m_pBattle_Terrain = { nullptr };

	_vector							m_pPlayer_Pos{};

	_vector							m_vMnemyDigimonPos[5] = {};
	_vector							m_vPlayerDigimonPos[5] = {};
	_vector							m_vPlayerBattlePos{};

	_float							m_fEnemyDigimon_Look{};
	_float							m_fPlayer_Look{};

	CGameObject*					m_pBattle = { nullptr };


private:

public:
	virtual void Free() override;
};

NS_END