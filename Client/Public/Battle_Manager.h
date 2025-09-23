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

public:
	_bool Get_Battle() { return m_bBattleOn; }
	void Set_Battle(_bool Battle) {	m_bBattleOn = Battle; }

	void Battle_System();

	void Digimon1_Skill();
	void Digimon2_Skill();
	void Digimon3_Skill();



private:
	_bool m_bBattleOn = { false };

	CGameInstance* m_pGameInstance = { nullptr };

	vector<class CContainerObject*> m_pMyDigimon;
	vector<class CContainerObject*> m_pEnemyDigimon;

private:

public:
	virtual void Free() override;
};

NS_END