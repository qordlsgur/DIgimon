#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Client)

class CSkillObject abstract : public CGameObject
{
protected:
	CSkillObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSkillObject(const CSkillObject& Prototype);
	virtual ~CSkillObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void Set_TargetPos(_fvector Pos) { m_pTarget_pos = Pos; }
	void Set_TrakPosition(_float TrakPosition) { m_fTrakPosition = TrakPosition; }

private:
	_vector		m_pTarget_pos{};
	_float		m_fSpeed{};
	_float		m_fTrakPosition{};


public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

NS_END