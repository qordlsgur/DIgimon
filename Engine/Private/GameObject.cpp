#include "GameObject.h"
#include "GameInstance.h"

CGameObject::CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
	, m_pGameInstance { CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CGameObject::CGameObject(const CGameObject& Prototype)
	: m_pDevice{ Prototype.m_pDevice }
	, m_pContext{ Prototype.m_pContext }
	, m_pGameInstance{ Prototype.m_pGameInstance }
	, m_isDead { Prototype.m_isDead }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CGameObject::Initialize_Prototype()
{	
	return S_OK;
}

HRESULT CGameObject::Initialize(void* pArg)
{
	m_pTransformCom = CTransform::Create(m_pDevice, m_pContext);
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	// transform의 Initialize까지 실행을 해주는데 pArg값을 던져준다.
	if (FAILED(m_pTransformCom->Initialize(pArg)))
		return E_FAIL;


	if (nullptr != pArg)
	{
		GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
	}

	m_Components.emplace(g_strTransformTag, m_pTransformCom);


	Safe_AddRef(m_pTransformCom);

	return S_OK;
}

void CGameObject::Priority_Update(_float fTimeDelta)
{
}

void CGameObject::Update(_float fTimeDelta)
{
}

void CGameObject::Late_Update(_float fTimeDelta)
{
}

HRESULT CGameObject::Render()
{
	return S_OK;
}


// 지금 이 오브젝트가 그 컴포넌트를 들고 있는지 없는지 확인을 해서 값이 있으면
// 그 값을 넘겨주고 없으면 nullptr을 넘긴다.
CComponent* CGameObject::Find_Component(const _wstring& strComponentTag)
{
	auto	iter = m_Components.find(strComponentTag);
	if (iter == m_Components.end())
		return nullptr;

	return iter->second;
}

_vector CGameObject::Get_Transform()
{
	return m_pTransformCom->Get_State(STATE::POSITION);
}

void CGameObject::Set_Matrix(_float4x4 Matrix)
{
	m_pTransformCom->Set_WorldMatrix(Matrix);
}

void CGameObject::OnClick()
{
}

// 이 함수는 Component를 추가 할 때 원본을 찾아서 원본이 있으면 추가를 하고
// 원본이 없으면 종료시킨다.
HRESULT CGameObject::Add_Component(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strComponentTag, CComponent** ppOut, void* pArg)
{
	auto it = m_Components.find(strComponentTag);
	if (it != m_Components.end())
	{
		Safe_Release(it->second);  // 참조 카운트 감소 및 삭제
		m_Components.erase(it);
	}

	// 새로운 컴포넌트 생성
	CComponent* pComponent = dynamic_cast<CComponent*>(
		m_pGameInstance->Clone_Prototype(PROTOTYPE::COMPONENT,
			iPrototypeLevelIndex,
			strPrototypeTag,
			pArg));
	if (nullptr == pComponent)
		return E_FAIL;

	// Map에 추가하고 레퍼런스 카운트 증가
	m_Components.emplace(strComponentTag, pComponent);
	*ppOut = pComponent;
	Safe_AddRef(pComponent);
	return S_OK;
}

void CGameObject::Free()
{
	__super::Free();

	for (auto& Pair : m_Components)
		Safe_Release(Pair.second);

	m_Components.clear();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
