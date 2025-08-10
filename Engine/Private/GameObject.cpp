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
	// 객체가 사본으로 만들어 지는건 이제 오브젝트를 생성하는데
	// 그때 바로 TransformComponent를 생성하기 위해서 바로 Create를 박아둔다.
	// Create에 값을 채워주고 만약 값이 nullptr이면 실패한다.
	m_pTransformCom = CTransform::Create(m_pDevice, m_pContext);
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	// transform의 Initialize까지 실행을 해주는데 pArg값을 던져준다.
	if (FAILED(m_pTransformCom->Initialize(pArg)))
		return E_FAIL;

	// pArg가 있으면 값을 넘겨줌
	// 위에서 parg를 선언하고 밑에도 선언하면 두번 하는거 아니야 라고 생각 할 수 있는데
	// 위에 Transform에서 있는거만 들고가고 없는건 안들고감
	// 그리고 값을 Trasnform에 넘기는게 아니고 자기 자신이 들고있는것 뿐
	if (nullptr != pArg)
	{
		GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
	}

	// 그리고 컴포넌트가 처음부터 생성이 되어서 바로 넣어준다.
	m_Components.emplace(g_strTransformTag, m_pTransformCom);

	// 그리고 레퍼런스 카운트를 1 증가 시킨다.
	// 원래는 컴포넌트를 AddCount에서 증가를 시키는데 Add로 선언한게 아니라 바로 생성을
	// 해버려서 여기서 바로 값을 증가 시켜줬다.
	// 원래 Map에 넣는건 AddComponent에서 해준디.
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

void CGameObject::OnClick()
{
}


// 이 함수는 Component를 추가 할 때 원본을 찾아서 원본이 있으면 추가를 하고
// 원본이 없으면 종료시킨다.
HRESULT CGameObject::Add_Component(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strComponentTag, CComponent** ppOut, void* pArg)
{
	// 지금 오브젝트가 이 컴포넌트를 가지고 있나 없나 확인을 해서 만약 가지고 있으면 
	// 종료 시킨다.
	if (nullptr != Find_Component(strComponentTag))
		return E_FAIL;

	// 그리고 컴포넌트가 없으면 추가를 한다.
	// 만약 원본을 몾찾으면 종료함
	CComponent* pComponent = dynamic_cast<CComponent*>(
		m_pGameInstance->Clone_Prototype(PROTOTYPE::COMPONENT,
			iPrototypeLevelIndex, strPrototypeTag, pArg));
	if (nullptr == pComponent)
		return E_FAIL;

	// 생성이 완료가 되면 이제 Map에 추가를 하고 레퍼런스 카운트를 1 증가 시킨다. 
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
