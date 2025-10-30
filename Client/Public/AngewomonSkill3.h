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

class CAngewomonSkill3 final : public CSkillObject
{
private:
	CAngewomonSkill3(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAngewomonSkill3(const CAngewomonSkill3& Prototype);
	virtual ~CAngewomonSkill3() = default;

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
	CPartObject* m_pSkillModel1 = { nullptr };
	CPartObject* m_pSkillModel2 = { nullptr };
	CPartObject* m_pSkillModel3 = { nullptr };
	CCollider* m_pColliderCom = { nullptr };
	CContainerObject* m_pPlayer = { nullptr };
	class CInteraction_Manager* m_pInteraction_Manager = { nullptr };

	_float _fTime{};
	_vector m_vFirstPosigion{};

private:
	HRESULT Ready_PartObjects();
	HRESULT Ready_SkillObjects();
	HRESULT Ready_SkillObjects2();
	HRESULT Ready_SkillObjects3();

public:
	static CAngewomonSkill3* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END