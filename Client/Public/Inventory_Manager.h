#pragma once

#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Client)

class CInventory_Manager final : public CBase
{
public:
	DECLARE_SINGLETON(CInventory_Manager);

private:
	CInventory_Manager();
	virtual ~CInventory_Manager() = default;

public:
	HRESULT Initialize();

public:

	virtual void Free() override;

};

NS_END