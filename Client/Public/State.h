#pragma once

#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Engine)
class CPartObject;
NS_END

NS_BEGIN(Client)

class CState abstract : public CBase
{	
public:
	CState();
	virtual ~CState() = default;

public:
	virtual void Enter(class CPartObject* pPart, _uint iIndex, _bool Dash = false, _bool Loop = true) = 0;
	virtual void Update(_float fTimedelta) = 0;
	virtual void Exit() = 0;

	

protected: 
	_float m_fTime{};
	_bool m_bDash = { false };
	_bool m_bLoop = { false };
	CPartObject* m_pPart;
	_uint m_iIndex{};


public:
	virtual void Free() override;
};

NS_END