#pragma once
#include "Client_Defines.h"
#include "SkillObject.h"

NS_BEGIN(Engine)
class CCollider;
class CTexture;
class CContainerObject;
NS_END

NS_BEGIN(Client)

class CMetalgarumonSkill3 final : public CSkillObject
{
private:
	CMetalgarumonSkill3(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMetalgarumonSkill3(const CMetalgarumonSkill3& Prototype);
	virtual ~CMetalgarumonSkill3() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual _int Get_Damage() override;
	void Set_Matrix(_matrix mouth) { m_mMouth = mouth; }
private:
	CCollider* m_pColliderCom = { nullptr };
	CContainerObject* m_pPlayer = { nullptr };
	class CInteraction_Manager* m_pInteraction_Manager = { nullptr };
	CPartObject* m_pSkillModel1 = { nullptr };

	_matrix m_mMouth{};
	_float m_fEndTime{};
	vector<HITINFO> m_Info;
	_int iHitCount{};
private:
	HRESULT Ready_PartObjects();
	HRESULT Ready_SkillObjects();

public:
	static CMetalgarumonSkill3* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END