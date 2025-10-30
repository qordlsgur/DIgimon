#pragma once
#include "Client_Defines.h"
#include "SkillObject.h"

NS_BEGIN(Engine)
class CCollider;
class CTexture;
class CContainerObject;
NS_END

NS_BEGIN(Client)

class CMetalgreymonSkill3 final : public CSkillObject
{
private:
	CMetalgreymonSkill3(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMetalgreymonSkill3(const CMetalgreymonSkill3& Prototype);
	virtual ~CMetalgreymonSkill3() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual _int Get_Damage() override;
	void Set_Hatch(_matrix LHatch, _matrix RHatch) { m_mLeftHatch = LHatch; m_mRightHatch = RHatch; }

private:
	CCollider* m_pColliderCom = { nullptr };
	CContainerObject* m_pPlayer = { nullptr };
	CPartObject* m_pSkillModel1 = { nullptr };
	CPartObject* m_pSkillModel2 = { nullptr };
	CPartObject* m_pSkillModel3 = { nullptr };
	CPartObject* m_pSkillModel4 = { nullptr };
	class CInteraction_Manager* m_pInteraction_Manager = { nullptr };

	_matrix m_mLeftHatch{};
	_matrix m_mRightHatch{};
	_float m_fTime{};
private:
	HRESULT Ready_PartObjects();
	HRESULT Ready_SkillObjects();
	HRESULT Ready_SkillObjects2();
	HRESULT Ready_SkillObjects3();

public:
	static CMetalgreymonSkill3* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END