#include "Interaction.h"
#include "GameInstance.h"
#include "Interaction_Manager.h"

CInteraction::CInteraction(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice ,pContext }
{
}

CInteraction::CInteraction(const CInteraction& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CInteraction::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CInteraction::Initialize(void* pArg)
{
	m_pInteraction_Manager = CInteraction_Manager::GetInstance();

	return S_OK;
}

void CInteraction::Priority_Update(_float fTimeDelta)
{
	m_pInteraction_Manager->Priority_Update();
}

void CInteraction::Update(_float fTimeDelta)
{
	m_pInteraction_Manager->Update();
}

void CInteraction::Late_Update(_float fTimeDelta)
{
	m_pInteraction_Manager->Late_Update();
}

HRESULT CInteraction::Render()
{
	return S_OK;
}

CInteraction* CInteraction::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CInteraction* pInstance = new CInteraction(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CInteraction");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CInteraction::Clone(void* pArg)
{
	CInteraction* pInstance = new CInteraction(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDigivice");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CInteraction::Free()
{
	__super::Free();


}
