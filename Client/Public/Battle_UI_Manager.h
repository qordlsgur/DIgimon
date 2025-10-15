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
	HRESULT Create_TimeLine();

	void Set_Battle_Turn_Order(deque<CContainerObject*>Digimon_Turn_Order);

	void Set_MyDigimon(DIGIMON_INFO* MyDigimon);
	void Set_Enemy_Digimon(DIGIMON_INFO* EnemyDigimon);
		
	void Set_Timeline_Turn_Order();

	void Turn_Start();
	void Turn_End();

private:
	CGameInstance* m_pGameInstance = { nullptr };

	deque<CContainerObject*>		m_pDigimon_Turn_Order;

	vector<DIGIMON_INFO*>		m_MyDigimon_Info;
	vector<DIGIMON_INFO*>		m_EnemyDigimon_Info;

	deque<class CBattle_Timeline*>	m_pTimeLines;
	vector<class CBattle_Timeline*> m_pTimeLine_array;
	class CBattle_Timeline* m_pTimeLine = { nullptr };
	class CBattle_Turn* m_pTurn = { nullptr };
	
	class CBattle_Timeline* m_pCurrent_TimeLine = { nullptr };

	_float2 m_fTurn_Panel[6];

public:
	virtual void Free() override;
};

NS_END