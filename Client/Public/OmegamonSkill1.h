#pragma once
#include "Client_Defines.h"
#include "SkillObject.h"

NS_BEGIN(Engine)
class CCollider;
class CTexture;
NS_END

NS_BEGIN(Client)

class COmegamonSkill1 final : public CSkillObject
{
private:
	COmegamonSkill1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	COmegamonSkill1(const COmegamonSkill1& Prototype);
	virtual ~COmegamonSkill1() = default;

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

	class CInteraction_Manager* m_pInteraction_Manager = { nullptr };

	_vector Pos{};

private:
	HRESULT Ready_PartObjects();


public:
	static COmegamonSkill1* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END