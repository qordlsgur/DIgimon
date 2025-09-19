#pragma once
#include "Client_Defines.h"
#include "ContainerObject.h"
#include "StateMachine.h"

NS_BEGIN(Client)

class CAngewomon final : public CContainerObject
{
private:
	CAngewomon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAngewomon(const CAngewomon& Prototype);
	virtual ~CAngewomon() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	class CPartObject* m_pPart_Body = { nullptr };
	class CDigimon_Manager* m_pDigimon_Manager = { nullptr };
	class CStateMachine* m_pFsm = { nullptr };

	_bool m_bMove = false;
	_bool Skill = false;


private:
	HRESULT Ready_PartObjects();

public:
	static CAngewomon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END