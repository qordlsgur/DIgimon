#pragma once
#include "Client_Defines.h"
#include "SkillObject.h"

NS_BEGIN(Engine)
class CCollider;
class CTexture;
NS_END

NS_BEGIN(Client)

class CLeomonSkill3 final : public CSkillObject
{
private:
	CLeomonSkill3(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLeomonSkill3(const CLeomonSkill3& Prototype);
	virtual ~CLeomonSkill3() = default;

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

	class CInteraction_Manager* m_pInteraction_Manager = { nullptr };

	_vector Pos{};
private:
	HRESULT Ready_PartObjects();
	HRESULT Ready_SkillObjects();


public:
	static CLeomonSkill3* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END