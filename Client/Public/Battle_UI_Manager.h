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
	void Set_MyDigimon(DIGIMON_INFO* MyDigimon);
	void Set_Enemy_Digimon(DIGIMON_INFO* EnemyDigimon);

private:

	vector<DIGIMON_INFO*> m_MyDigimon_Info;
	vector<DIGIMON_INFO*> m_EnemyDigimon_Info;

public:
	virtual void Free() override;
};

NS_END