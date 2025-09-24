#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)

NS_END

NS_BEGIN(Client)

class CInteraction final : public CGameObject
{
private:
	CInteraction(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CInteraction(const CInteraction& Prototype);
	virtual ~CInteraction() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	class CInteraction_Manager* m_pInteraction_Manager = { nullptr };

public:
	static CInteraction* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END