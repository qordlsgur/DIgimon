#pragma once

#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;
class CGameObject;
class CContainerObject;
class CCollider;
NS_END

NS_BEGIN(Client)

class CInteraction_Manager final : public CBase
{
public:
	DECLARE_SINGLETON(CInteraction_Manager);
private:
	CInteraction_Manager();
	virtual ~CInteraction_Manager() = default;

public:
	HRESULT Initialize();
	void Priority_Update();
	void Update();
	void Late_Update();

public:
	void Set_Player(CContainerObject* pPlayer);
	void Set_Player_Collider(CCollider* pCollider);
	void Set_Enemy_Digimon(CContainerObject* Enemy);



	void Set_Battle_Digimon(CContainerObject* Battle_Digimon);
	void Battle_End();
private:
	//CGameInstance* m_pGameInstance = { nullptr };
	CContainerObject* m_pPlayer = { nullptr };
	CCollider* m_pPlayer_Collider = { nullptr };

	vector<CContainerObject*> m_pEnemy_Digimon;

	vector<CContainerObject*> m_pBattle_Digimon_Collider;

public:
	virtual void Free() override;
};

NS_END