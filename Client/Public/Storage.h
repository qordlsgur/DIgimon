#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)

NS_END

NS_BEGIN(Client)

class CStorage final : public CUIObject
{
private:
	CStorage(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStorage(const CStorage& Prototype);
	virtual ~CStorage() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	//vector<>		m_vSlot;

private:
	HRESULT Ready_Components();

public:
	static CStorage* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END