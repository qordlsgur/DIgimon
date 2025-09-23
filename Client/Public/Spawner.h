#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CContainerObject;
NS_END

NS_BEGIN(Client)
class CSpawner final : public CGameObject
{
public:
	typedef struct Spawner_Pos
	{
		_vector SpawnerPos{};
		_wstring strPrototypeTag;
	}SPAWNER_POS;
private:
	CSpawner(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSpawner(const CSpawner& Prototype);
	virtual ~CSpawner() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	_vector m_pSpawnerPos{};
	_wstring m_strTag;

	_uint m_iMaxMonster{};
	_uint m_iMonsterCount{};

	CContainerObject* m_pMonster = { nullptr };

	vector<CContainerObject*> m_pMonsters;

public:
	static CSpawner* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END