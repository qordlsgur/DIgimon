#pragma once
#include "Client_Defines.h"
#include "SkillObject.h"

NS_BEGIN(Engine)
class CCollider;
class CTexture;
class CContainerObject;
NS_END

NS_BEGIN(Client)

class CMetalgreymonSkill2 final : public CSkillObject
{
private:
	CMetalgreymonSkill2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMetalgreymonSkill2(const CMetalgreymonSkill2& Prototype);
	virtual ~CMetalgreymonSkill2() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual _int Get_Damage() override;

private:
	CCollider* m_pColliderCom = { nullptr };
	CPartObject* m_pSkillModel1 = { nullptr };
	CPartObject* m_pSkillModel2 = { nullptr };
	CPartObject* m_pSkillModel3 = { nullptr };
	CPartObject* m_pSkillModel4 = { nullptr };
	CContainerObject* m_pPlayer = { nullptr };
	class CInteraction_Manager* m_pInteraction_Manager = { nullptr };

	_float m_fTime{};

private:
	HRESULT Ready_PartObjects();
	HRESULT Ready_SkillObjects();
	HRESULT Ready_SkillObjects2();

public:
	static CMetalgreymonSkill2* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END