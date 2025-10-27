#pragma once
#include "Client_Defines.h"
#include "SkillObject.h"

NS_BEGIN(Engine)
class CCollider;
class CPartObject;
class CContainerObject;
NS_END

NS_BEGIN(Client)

class CDevilmonSkill3 final : public CSkillObject
{
private:
	CDevilmonSkill3(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDevilmonSkill3(const CDevilmonSkill3& Prototype);
	virtual ~CDevilmonSkill3() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual _int Get_Damage() override;
	void Set_Hand(_matrix Left, _matrix Right) { m_vLeft = Left; m_vRight = Right; }
	void Set_Charge(_bool Charge) { m_bCharge = Charge; }
	void pentagram(_bool pentagram) { m_bpentagram = pentagram; }
private:
	CCollider* m_pColliderCom = { nullptr };
	CContainerObject* m_pPlayer = { nullptr };
	class CDevilmonSkill3_Part2* m_pLeftHand = { nullptr };
	class CDevilmonSkill3_Part3* m_pRightHand = { nullptr };
	class CInteraction_Manager* m_pInteraction_Manager = { nullptr };

	_bool m_bpentagram = { false };
	_bool m_bCharge = { false };
	_matrix m_vLeft{};
	_matrix m_vRight{};
private:
	HRESULT Ready_PartObjects();
	HRESULT Ready_SkillObjects();
	HRESULT Ready_Skill2Objects();
	HRESULT Ready_Skill3Objects();

public:
	static CDevilmonSkill3* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END