#pragma once

#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;
class CContainerObject;
NS_END

NS_BEGIN(Client)

class CInteraction_Manager final : public CBase
{
public:
	DECLARE_SINGLETON(CInteraction_Manager);
private:
	CInteraction_Manager();
	virtual ~CInteraction_Manager() = default;

public:


private:
	CGameInstance* m_pGameInstance = { nullptr };

private:


public:
	virtual void Free() override;
};

NS_END