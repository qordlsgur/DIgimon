#pragma once

#include "Client_Defines.h"
#include "State.h"

NS_BEGIN(Client)

class CRoar final : public CState
{
public:
	CRoar();
	virtual ~CRoar() = default;
public:
	void Enter(class CPartObject* pPart, _uint iIndex, _bool Dash = false, _bool Loop = true) override;
	void Update(_float fTimedelta) override;
	void Exit() override;

public:
	virtual void Free() override;

};

NS_END