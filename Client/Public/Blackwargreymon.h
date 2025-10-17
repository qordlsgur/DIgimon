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

class CBlackwargreymon final : public CContainerObject
{
private:
	CBlackwargreymon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBlackwargreymon(const CBlackwargreymon& Prototype);
	virtual ~CBlackwargreymon() = default;

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

	void Set_Damage(_int Damage);

private:
	CCollider* m_pColliderCom = { nullptr };
	CPartObject* m_pPart_Body = { nullptr };

	class CDigimon_Manager* m_pDigimon_Manager = { nullptr };
	class CStateMachine* m_pFsm = { nullptr };

	_bool m_bMove = false;
	_int m_iDamage{};
	_bool m_bisHit = { false };

private:
	HRESULT Ready_PartObjects();

public:
	static CBlackwargreymon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END