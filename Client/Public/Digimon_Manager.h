#pragma once

#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Engine)

NS_END

NS_BEGIN(Client)

class CDigimon_Manager final : public CBase
{
public:
	DECLARE_SINGLETON(CDigimon_Manager);

private:
	CDigimon_Manager();
	virtual ~CDigimon_Manager() = default;

public:
	HRESULT Initialize();

public:
	void Party_In();
	void Party_Out();

private:
	class CPartyUHD* m_pPartyUHD = { nullptr };
	class CDigivice* m_pDigivice = { nullptr };
	class CDigimon_Storage* m_pDigimon_Storage = { nullptr };

public:
	virtual void Free() override;
};

NS_END