#pragma once
#include "Client_Defines.h"
#include "SkillObject.h"

NS_BEGIN(Engine)
class CCollider;
class CTexture;
class CContainerObject;
class CPartObject;
NS_END

NS_BEGIN(Client)

class CLeomonSkill2 final : public CSkillObject
{
private:
	CLeomonSkill2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLeomonSkill2(const CLeomonSkill2& Prototype);
	virtual ~CLeomonSkill2() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Pos(_float fX, _float fY);

	virtual _int Get_Damage() override;
private:
	CCollider* m_pColliderCom = { nullptr };
	CPartObject* m_pSkillModel1 = { nullptr };
	CPartObject* m_pSkillModel2 = { nullptr };
	CContainerObject* m_pPlayer = { nullptr };
	class CInteraction_Manager* m_pInteraction_Manager = { nullptr };

	_vector Pos{};

private:
	HRESULT Ready_PartObjects();
	HRESULT Ready_SkillObjects();
	HRESULT Ready_SkillObjects2();

public:
	static CLeomonSkill2* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END