#pragma once
#include "Client_Defines.h"
#include "SkillObject.h"

NS_BEGIN(Engine)
class CCollider;
class CTexture;
NS_END

NS_BEGIN(Client)

class CBlackwargreymonSkill3 final : public CSkillObject
{
private:
	CBlackwargreymonSkill3(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBlackwargreymonSkill3(const CBlackwargreymonSkill3& Prototype);
	virtual ~CBlackwargreymonSkill3() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Pos(_float fX, _float fY);

private:


private:
	CCollider* m_pColliderCom = { nullptr };

	_float m_fX;
	_float m_fY;

	_matrix m;

private:
	HRESULT Ready_PartObjects();


public:
	static CBlackwargreymonSkill3* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END