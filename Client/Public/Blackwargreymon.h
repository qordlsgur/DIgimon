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

	virtual void Creat_Skill(_int SkillNum) override;

public:
	virtual _int Intersect(class CCollider* pPlayer_Collider) override;

	//void DamageUp(_float fTimeDelta);

private:
	CCollider* m_pColliderCom = { nullptr };
	CPartObject* m_pPart_Body = { nullptr };
	class CSkillObject* m_pSkill1 = { nullptr };
	class CDigimon_Manager* m_pDigimon_Manager = { nullptr };
	class CStateMachine* m_pFsm = { nullptr };

	_bool m_bMove = { false };
	_bool m_bisHit = { false };
	_float m_fTime{};
	_float m_fFontUp{};
	_float m_fRandom{};

private:
	HRESULT Ready_PartObjects();

public:
	static CBlackwargreymon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END