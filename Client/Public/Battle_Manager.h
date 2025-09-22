#pragma once

#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Engine)
class CGameObject;
NS_END

NS_BEGIN(Client)
class CBattle_Manager final : public CBase
{
public:
	DECLARE_SINGLETON(CBattle_Manager);
private:
	CBattle_Manager();
	virtual ~CBattle_Manager() = default;

public:
	HRESULT Initialize();

private:

public:
	virtual void Free() override;
};

NS_END