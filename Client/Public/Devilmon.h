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

class CDevilmon final : public CContainerObject
{
private:
	CDevilmon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDevilmon(const CDevilmon& Prototype);
	virtual ~CDevilmon() = default;

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
	virtual void Creat_Skill(_int SkillNum) override;

public:
	virtual _int Intersect(class CCollider* pPlayer_Collider) override;

private:
	CCollider* m_pColliderCom = { nullptr };
	CPartObject* m_pPart_Body = { nullptr };
	class CSkillObject* m_pSkill = { nullptr };
	class CDigimon_Manager* m_pDigimon_Manager = { nullptr };
	class CStateMachine* m_pFsm = { nullptr };

	_bool m_bMove = false;
	_bool m_bSkillOn = { false };
	_matrix m_vLeftHand{};
	_matrix m_vRightHand{};

private:
	HRESULT Ready_PartObjects();

public:
	static CDevilmon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END