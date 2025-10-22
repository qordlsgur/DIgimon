#pragma once

#include "Transform.h"

NS_BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
public:
	// 게임 오브젝트로 만드는 오브젝트들은 전부 다 Transform을 받아오기 떄문에
	// 속도랑 회전 값을 넘겨 주기 위해서 값을 넘겨주는 친구이다. 
typedef struct tagGameObjectDesc : public CTransform::TRANSFORM_DESC
{

}GAMEOBJECT_DESC;
typedef struct tagPosition
{
	_matrix fmatrix;
}POSITION;

protected:
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject(const CGameObject& Prototype);
	virtual ~CGameObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	_bool isDead() const {
		return m_isDead;
	}

	void Set_isDead(_bool Dead) { m_isDead = Dead; }

	class CComponent* Find_Component(const _wstring& strComponentTag);

	virtual _vector Get_Transform();
	virtual void Set_Matrix(_float4x4 Matrix);


public:
	virtual void OnClick();

protected:
	int											m_iData;
	ID3D11Device*								m_pDevice = { nullptr };
	ID3D11DeviceContext*						m_pContext = { nullptr };
	class CGameInstance*						m_pGameInstance = { nullptr };
	_bool										m_isDead = { false };
	class CTransform*							m_pTransformCom = { nullptr };

	map<const _wstring, class CComponent*>		m_Components;

protected:

	HRESULT Add_Component(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, 
		const _wstring& strComponentTag, CComponent** ppOut, void* pArg = nullptr);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

NS_END