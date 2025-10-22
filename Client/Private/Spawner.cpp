#include "Spawner.h"
#include "GameInstance.h"
#include "ContainerObject.h"
#include "Interaction_Manager.h"
#include "Battle_Manager.h"

CSpawner::CSpawner(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CSpawner::CSpawner(const CSpawner& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CSpawner::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSpawner::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC	Desc{};
	Desc.fRotationPerSec = XMConvertToRadians(180.0f);
	Desc.fSpeedPerSec = 50.f;

	SPAWNER_POS* pPos = static_cast<SPAWNER_POS*>(pArg);
	m_pSpawnerPos = pPos->SpawnerPos;
	m_strTag = pPos->strPrototypeTag;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	m_pInteraction_Manager = CInteraction_Manager::GetInstance();
	m_pBattlea_Manager = CBattle_Manager::GetInstance();
	m_pTransformCom->Set_State(STATE::POSITION, m_pSpawnerPos);

	m_iMaxMonster = 5;

	return S_OK;
}

void CSpawner::Priority_Update(_float fTimeDelta)
{
}

void CSpawner::Update(_float fTimeDelta)
{
	if (m_iMonsterCount < m_iMaxMonster)
	{
		for (_uint i = 0; i < m_iMaxMonster; ++i)
		{
			m_pMonster = static_cast<CContainerObject*>(m_pGameInstance->Add_GameObject_ToLayer_ToCreate(ENUM_CLASS(LEVEL::GAMEPLAY),
				m_strTag, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Spawn_Monster")));

			m_pMonsters.push_back(m_pMonster);

			m_iMonsterCount++;
			m_pInteraction_Manager->Set_Enemy_Digimon(m_pMonster);
			m_pMonsters[i]->Set_Monster(true);
			m_pMonster->Set_Position(m_pSpawnerPos.m128_f32[0] + m_pGameInstance->Random(1.f, 150.f), m_pSpawnerPos.m128_f32[2] + m_pGameInstance->Random(1.f, 150.f));
		}
	}
	//if (m_pBattlea_Manager->Get_Battle())
	//{
	//	for (_uint i = 0; i < m_iMaxMonster; ++i)
	//	{
	//		m_pMonsters[i]->Set_Life(false);
	//	}
	//}
	//else
	//{
	//	for (_uint i = 0; i < m_iMaxMonster; ++i)
	//	{
	//		m_pMonsters[i]->Set_Life(true);
	//	}
	//}
}

void CSpawner::Late_Update(_float fTimeDelta)
{
}

HRESULT CSpawner::Render()
{
	return S_OK;
}

CSpawner* CSpawner::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSpawner* pInstance = new CSpawner(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSpawner");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSpawner::Clone(void* pArg)
{
	CSpawner* pInstance = new CSpawner(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSpawner");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpawner::Free()
{
	__super::Free();

	m_pMonsters.clear();
}
