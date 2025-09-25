#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CContainerObject;
NS_END

NS_BEGIN(Client)

class CBattle final : public CGameObject
{
private:
	CBattle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBattle(const CBattle& Prototype);
	virtual ~CBattle() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	class CBattle_Manager* m_pBattle_Manager = { nullptr };
	class CDigimon_Manager* m_pDigimon_Manager = { nullptr };

	_bool m_Battle = { false };

public:
	static CBattle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END