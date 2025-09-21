#include "Camera_Manager.h"
#include "GameInstance.h"
#include "GameObject.h"


IMPLEMENT_SINGLETON(CCamera_Manager)

CCamera_Manager::CCamera_Manager()
{
}


HRESULT CCamera_Manager::Initialize()
{
	return S_OK;
}

void CCamera_Manager::Add_Camera(CGameObject* pCamera)
{
	m_Cameras.push_back(pCamera);
}

void CCamera_Manager::Add_Player(CGameObject* pPlayer)
{
	m_pPlayer = pPlayer;
}

void CCamera_Manager::PlayerPos(_vector& Pos)
{
	if (m_pPlayer != nullptr)
	{
		Pos = m_pPlayer->Get_Transform();
	}
}

_bool CCamera_Manager::HasPlayer()
{
	if (m_pPlayer == nullptr)
		return false;

	return true;
}

void CCamera_Manager::Free()
{
	__super::Free();

}
