#include "Object_Manager.h"
#include "GameInstance.h"

#include "Layer.h"
#include "GameObject.h"

CObject_Manager::CObject_Manager()
	: m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

CComponent* CObject_Manager::Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);
	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_Component(strComponentTag, iIndex);
}

CComponent* CObject_Manager::Get_PartObject_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strPartTag, const _wstring& strComponentTag, _uint iIndex)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);
	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_PartObject_Component(strPartTag, strComponentTag, iIndex);
}

HRESULT CObject_Manager::Initialize(_uint iNumLevels)
{
	m_iNumLevels = iNumLevels;

	m_pLayers = new map<const _wstring, CLayer*>[iNumLevels];
	m_pNonLayers = new map<const _wstring, CLayer*>[iNumLevels];

	return S_OK;
}

HRESULT CObject_Manager::Add_GameObject_ToLayer(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLayerLevelIndex, const _wstring& strLayerTag, void* pArg, _bool Anim)
{
	CGameObject* pGameObject = dynamic_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, iPrototypeLevelIndex, strPrototypeTag, pArg));
	if (nullptr == pGameObject)
		return E_FAIL;

	CLayer* pLayer = Find_Layer(iLayerLevelIndex, strLayerTag);
	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();

		pLayer->Add_GameObject(pGameObject);

		m_pLayers[iLayerLevelIndex].emplace(strLayerTag, pLayer);
	}
	else
		pLayer->Add_GameObject(pGameObject);

	return S_OK;
}

CGameObject* CObject_Manager::Add_GameObject_ToLayer_ToCreate(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLayerLevelIndex, const _wstring& strLayerTag, void* pArg, _bool Anim)
{
	CGameObject* pGameObject = dynamic_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, iPrototypeLevelIndex, strPrototypeTag, pArg));
	if (nullptr == pGameObject)
		return nullptr;

	if (true == Anim)
	{
		CLayer* pLayer = Find_Layer(iLayerLevelIndex, strLayerTag);
		if (nullptr == pLayer)
		{
			pLayer = CLayer::Create();
			pLayer->Add_GameObject(pGameObject);
			m_pLayers[iLayerLevelIndex].emplace(strLayerTag, pLayer);
		}
		else
		{
			pLayer->Add_GameObject(pGameObject);
		}
	}

	else
	{
		CLayer* pLayer = Find_NonAnimLayer(iLayerLevelIndex, strLayerTag);
		if (nullptr == pLayer)
		{
			pLayer = CLayer::Create();
			pLayer->Add_NonGameObject(pGameObject);
			m_pNonLayers[iLayerLevelIndex].emplace(strLayerTag, pLayer);
		}
		else
		{
			pLayer->Add_NonGameObject(pGameObject);
		}
	}

	return pGameObject;
}

void CObject_Manager::Priority_Update(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->Priority_Update(fTimeDelta);
	}
}

void CObject_Manager::Update(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->Update(fTimeDelta);
	}
}

void CObject_Manager::Late_Update(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->Late_Update(fTimeDelta);
	}
}

void CObject_Manager::Clear(_uint iLevelIndex)
{
	if (!m_pLayers[iLevelIndex].empty())
	{
		for (auto& Pair : m_pLayers[iLevelIndex])
		{
			Pair.second->Clear();
			Safe_Release(Pair.second);
		}

		m_pLayers[iLevelIndex].clear();
	}

	if (!m_pNonLayers[iLevelIndex].empty())
	{
		for (auto& Pair : m_pNonLayers[iLevelIndex])
		{
			Pair.second->Clear();
			Safe_Release(Pair.second);
		}

		m_pNonLayers[iLevelIndex].clear();
	}
}

void CObject_Manager::Clear_DeadObj()
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Pair.second->Clear_DeadObj();
		}
	}
}


CLayer* CObject_Manager::Find_Layer(_uint iLayerLevelIndex, const _wstring& strLayerTag)
{
	if (iLayerLevelIndex >= m_iNumLevels)
		return nullptr;

	auto	iter = m_pLayers[iLayerLevelIndex].find(strLayerTag);
	if (iter == m_pLayers[iLayerLevelIndex].end())
		return nullptr;

	return iter->second;
}

CLayer* CObject_Manager::Find_NonAnimLayer(_uint iLayerLevelIndex, const _wstring& strLayerTag)
{
	if (iLayerLevelIndex >= m_iNumLevels)
		return nullptr;

	auto	iter = m_pNonLayers[iLayerLevelIndex].find(strLayerTag);
	if (iter == m_pNonLayers[iLayerLevelIndex].end())
		return nullptr;

	return iter->second;
}

CObject_Manager* CObject_Manager::Create(_uint iNumLevels)
{
	CObject_Manager* pInstance = new CObject_Manager();

	if (FAILED(pInstance->Initialize(iNumLevels)))
	{
		MSG_BOX("Failed to Created : CObject_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CObject_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);

	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Safe_Release(Pair.second);

		m_pLayers[i].clear();
	}

	Safe_Delete_Array(m_pLayers);

	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pNonLayers[i])
			Safe_Release(Pair.second);

		m_pNonLayers[i].clear();
	}

	Safe_Delete_Array(m_pNonLayers);
}
