#include "Interaction_Manager.h"
#include "GameInstance.h"
#include "ContainerObject.h"

IMPLEMENT_SINGLETON(CInteraction_Manager);

CInteraction_Manager::CInteraction_Manager()
{
}

HRESULT CInteraction_Manager::Initialize()
{
	return S_OK;
}

void CInteraction_Manager::Set_Player(CContainerObject* pPlayer)
{
	if (nullptr == pPlayer)
		m_pPlayer = pPlayer;
}

void CInteraction_Manager::Set_Player_Collider(CCollider* pCollider)
{
	if (nullptr == pCollider)
		m_pPlayer_Collider = pCollider;
}

void CInteraction_Manager::Set_Enemy_Digimon(CContainerObject* Enemy)
{
	m_pEnemy_Digimon.push_back(Enemy);
}

void CInteraction_Manager::Update()
{
	//for (_int i = 0; i < m_pEnemy_Digimon.size(); ++i)
	//{
	//	m_pEnemy_Digimon[i]->
	//}
}

void CInteraction_Manager::Free()
{
	__super::Free();
}
