#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Client)

class CSkillObject abstract : public CGameObject
{
public:
	typedef struct tagPosition
	{
		_vector m_vPosition;
		_vector m_vTargetPosition;
		_int	iDamage;
	}POSITION;
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
	void Set_TargetPos(_fvector Pos) { m_vTarget_pos = Pos; }
	void Set_TrakPosition(_float TrakPosition) { m_fTrakPosition = TrakPosition; }

	virtual void Set_Move(_vector Position);
	virtual void Move(_float fTimeDatel);
	virtual void Set_Hit(_bool Hit) { m_bHit = Hit; }
	virtual _bool Get_Hit() { return m_bHit; }
	virtual _int Get_Damage() = 0;
protected:
	_vector		m_vPosition{};
	_vector		m_vTarget_pos{};

	_float		m_fSpeed{};

	_float		m_fTrakPosition{};

	_int		m_iDamage{};

	_float		m_fX;
	_float		m_fY;

	_bool		m_bHit = { false };
public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

NS_END