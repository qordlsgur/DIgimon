#pragma once
#include "Client_Defines.h"
#include "SkillObject.h"

NS_BEGIN(Engine)
class CCollider;
class CTexture;
class CContainerObject;
NS_END

NS_BEGIN(Client)

class CWargreymonSkill2 final : public CSkillObject
{
private:
	CWargreymonSkill2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWargreymonSkill2(const CWargreymonSkill2& Prototype);
	virtual ~CWargreymonSkill2() = default;

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
	class CInteraction_Manager* m_pInteraction_Manager = { nullptr };

	_vector m_vFirst{};
	_float m_fTime{};

	_bool m_bStart = { false };
	_bool m_bFirst = { false };
	_bool m_bSize = { false };
	_bool m_bEnd = { false };
private:
	HRESULT Ready_PartObjects();

public:
	static CWargreymonSkill2* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END