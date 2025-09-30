#include "Battle.h"
#include "GameInstance.h"
#include "Battle_Manager.h"
#include "Digimon_Manager.h"


CBattle::CBattle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice ,pContext }
{
}

CBattle::CBattle(const CBattle& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CBattle::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBattle::Initialize(void* pArg)
{
	m_pBattle_Manager = CBattle_Manager::GetInstance();
    m_pDigimon_Manager = CDigimon_Manager::GetInstance();

    m_pBattle_Manager->Set_Battle(this);
	return S_OK;
}

void CBattle::Priority_Update(_float fTimeDelta)
{
	if (m_Battle)
	{
		m_pBattle_Manager->Priority_Update(fTimeDelta);
	}
}

void CBattle::Update(_float fTimeDelta)
{
	m_Battle = m_pBattle_Manager->Get_Battle();

	if (m_Battle)
	{
		m_pBattle_Manager->Update(fTimeDelta);
	}

}

void CBattle::Late_Update(_float fTimeDelta)
{
	if (m_Battle)
	{
		m_pBattle_Manager->Late_Update(fTimeDelta);
	}
}

HRESULT CBattle::Render()
{
	return S_OK;
}

CBattle* CBattle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBattle* pInstance = new CBattle(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBattle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBattle::Clone(void* pArg)
{
	CBattle* pInstance = new CBattle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDigivice");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBattle::Free()
{
	__super::Free();


}
