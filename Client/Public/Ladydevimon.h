#pragma once
#include "Client_Defines.h"
#include "ContainerObject.h"
#include "StateMachine.h"

NS_BEGIN(Client)

class CLadydevimon final : public CContainerObject
{
private:
	CLadydevimon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLadydevimon(const CLadydevimon& Prototype);
	virtual ~CLadydevimon() = default;

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
	virtual void Skill1() override;
	virtual void Skill2() override;
	virtual void Skill3() override;

	HRESULT Ready_PartObjects();

public:
	static CLadydevimon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END