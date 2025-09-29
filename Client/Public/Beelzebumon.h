#pragma once
#include "Client_Defines.h"
#include "ContainerObject.h"
#include "StateMachine.h"

NS_BEGIN(Engine)
class CNavigation;
class CPartObject;
class CCollider;
NS_END

NS_BEGIN(Client)

class CBeelzebumon final : public CContainerObject
{
private:
	CBeelzebumon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBeelzebumon(const CBeelzebumon& Prototype);
	virtual ~CBeelzebumon() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void UseSkill(_int Skill) override;
	virtual void Skill1() override;
	virtual void Skill2() override;
	virtual void Skill3() override;

public:
	virtual _int Intersect(class CCollider* pPlayer_Collider) override;

private:
	CPartObject* m_pPart_Body = { nullptr };
	CCollider* m_pColliderCom = { nullptr };

	class CDigimon_Manager* m_pDigimon_Manager = { nullptr };
	class CStateMachine* m_pFsm = { nullptr };

	_bool m_bMove = false;

private:
	HRESULT Ready_PartObjects();

public:
	static CBeelzebumon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END